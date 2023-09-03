#include "../commands.hpp"

/**
 * Checks if the provided channel name is valid.
 * A valid channel name should not contain spaces or commas, should not exceed 50 characters, and should start with '#'.
 * @param name  The channel name to be validated.
 * @return      True if the channel name is invalid, false otherwise.
 */
bool channel_name_not_valid(std::string name)
{
	if (name.find(" ") != std::string::npos || name.find(",") != std::string::npos)
		return (true);
	if (name.size() > 50)
		return (true);
	if (name[0] != '#')
		return (true);
	return (false);
}

/**
 * Parses the JOIN command and checks for errors.
 * @param line      Vector containing the command details.
 * @param all_name  Vector to store parsed channel names.
 * @param client    Reference to the client object initiating the command.
 * @return          Returns 0 if parsing is successful, or an error code corresponding to the type of error encountered.
 */
int join_parse_error(std::vector <std::string> line , std::vector <std::string> &all_name, Client &client)
{
	size_t pos = 0;
	std::string tmp_ch_name;

	if (line.size() < 2)
	{
		send(client.getFd(), "JOIN : Not enough parameters\r\n", 31, 0);
		return (ERR_NEEDMOREPARAMS);
	}
	pos = line[1].find(",");
	while (pos != std::string::npos)
	{
		tmp_ch_name = line[1].substr(0, pos);
		all_name.push_back(tmp_ch_name);
		line[1].erase(0, pos + 1);
		pos = line[1].find(",");
	}
	if (!line[1].empty())
		all_name.push_back(line[1]);
	for (unsigned int i = 0; i < all_name.size(); i++)
	{
		if (channel_name_not_valid(all_name[i]))
		{
			send(client.getFd(), "Invalid Channel Name\r\n", 23, 0);
			return (ERR_INVALIDCHNAME);
		}
	}
	return (0);
}

/**
 * Checks if the client is invited to a channel that has the invite-only mode set.
 * @param channel  Reference to the channel object.
 * @param client   Reference to the client object.
 * @return         Returns 0 if the client is invited or the channel is not in invite-only mode, 1 otherwise.
 */
int	checkInviteMode(Channel &channel, Client &client)
{
	if (channel.checkFlag('i') == true)
	{
		if (channel.checkInvited(client.getNickname()))
			return (0);
		return (1);
	}
	return (0);
}

/**
 * Allows a client to join an existing channel.
 * @param client    Reference to the client object.
 * @param server    Reference to the server object.
 * @param chan_nbr  Index of the channel in the server's channel list.
 * @return          Returns 0 if the join is successful, or an error code corresponding to the type of error encountered.
 */
int join_chan(Client &client, Server &server, int chan_nbr)
{
	std::string tmp;

	if (client.getChanNbr() >= 10)
	{
		tmp = server.channels[chan_nbr].getName() + " :You have joined too many channels\r\n";
		send(client.getFd(), tmp.c_str() , 37 + server.channels[chan_nbr].getName().size(), 0);
		return (ERR_TOOMANYCHANNELS);
	}
	if (server.channels[chan_nbr].isBlacklisted(client.getNickname()))
	{
		tmp = server.channels[chan_nbr].getName() + " :Cannot join channel (Banned)\r\n";
	 	send(client.getFd(), tmp.c_str(), tmp.length(), 0);
	 	return (ERR_BANNEDFROMCHAN);
	}
	if (server.channels[chan_nbr].checkFlag('l') == true)
	{
		std::cout << "Channel " << server.channels[chan_nbr].getName() << " is on +l mode" << std::endl;
		std::cout << "Flags: " << server.channels[chan_nbr].getFlags() << std::endl;
		if (server.channels[chan_nbr].getClients().size() >= (unsigned long) server.channels[chan_nbr].getMaxUsers())
		{
			tmp = server.channels[chan_nbr].getName() + " :Cannot join channel (+l)\r\n";
			send(client.getFd(), tmp.c_str(), tmp.length(), 0);
			return (ERR_CHANISFULL);
		}
	}

	// Check if channel is on invite mode
	if (checkInviteMode(server.channels[chan_nbr], client) == 1)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to join channel " << server.channels[chan_nbr].getName() << " but was not invited." << std::endl;
		std::string response = ":" + server.getName() + " 473 " + client.getNickname() + " " + server.channels[chan_nbr].getName() + " :Channel is on invite-only mode and you were not invited\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return (1);
	}

	server.channels[chan_nbr].addClients(client.getNickname());
	client.setChanNbr(client.getChanNbr() + 1);

	std::string name = server.channels[chan_nbr].getName();

	server.channels[chan_nbr].notifyUserJoin(client, server);

	std::string clientsList = server.channels[chan_nbr].getConnectedClientsAsString();

	std::string msg = ":" + server.getName() + " MODE " + name + " +nt\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " JOIN :" + name + "\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);

	if (server.channels[chan_nbr].getTopic() != "")
	{
		msg = ":" + server.getName() + " 332 " + client.getNickname() + " " + server.channels[chan_nbr].getName() +
			" " + server.channels[chan_nbr].getTopic() + "\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
	}
	else
	{
		msg = ":" + server.getName() + " 331 " + client.getNickname() + " " + name + " :No topic is set\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
	}
	msg = ":" + server.getName() + " 353 " + client.getNickname() + " = " + name + " :" + clientsList + "\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + server.getName() + " 366 " + client.getNickname() + " " + name + " :End of NAMES list\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	std::cout << "** INFO : " << intToIp(client.getAddr()) << " joined channel " << name << "." << std::endl;

	if (server.channels[chan_nbr].getBot() != NULL)
	{
		std::string botName = server.channels[chan_nbr].getBot()->getNickname();
		std::string channelName = server.channels[chan_nbr].getName();
		std::string messageText = "Hello, " + client.getNickname() + ", welcome to " + channelName + " !";
		std::string message = ":" + botName + "!~" + botName + "@127.0.0.1 PRIVMSG " + channelName + " :" + messageText + "\r\n";
		server.channels[server.findChannel(channelName)].sendToChannelMembers(server, message, "None");
		std::cout << "** BOT INFO : " << botName << " sent a message to channel " << channelName << "." << std::endl;
	}

	return (0);
}

/**
 * Creates a new channel and adds the client as its first member and operator.
 * @param client  Reference to the client object.
 * @param server  Reference to the server object.
 * @param name    Name of the new channel to be created.
 * @return        Returns 0 after creating the channel.
 */
int create_chan(Client &client, Server &server, std::string name)
{
	int old_length = server.channels.size();
	Channel new_channel = Channel(name);
	server.channels.push_back(new_channel);
	server.channels[old_length].addOperator(client.getNickname());

	std::string msg = ":" + server.getName() + " MODE " + name + " +nt\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " JOIN :" + name + "\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + server.getName() + " 332 " + client.getNickname() + " " + name + " :No topic is set\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + server.getName() + " 353 " + client.getNickname() + " = " + name + " :" + client.getNickname() + "\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + server.getName() + " 366 " + client.getNickname() + " " + name + " :End of NAMES list\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	std::cout << "** INFO : " << intToIp(client.getAddr()) << " created channel " << name << "." << std::endl;
	return (0);
}

/**
 * Processes the JOIN command to allow a client to join one or more channels.
 * If the channel does not exist, it will be created. If the channel exists, the client will be added to it.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the command.
 * @param cmd     Vector containing the command details.
 * @return        Returns 0 if the command is processed successfully, or an error code corresponding to the type of error encountered.
 */
int JOIN(Server &server, Client &client, std::vector<std::string> cmd)
{
	int rtn_value = 0;
	std::vector<std::string> all_name;

	rtn_value = join_parse_error(cmd, all_name, client);
	if (rtn_value != 0)
		return (rtn_value);
	for (unsigned long i = 0; i < all_name.size(); i++)
	{
		bool joined_existing_channel = false;
		for (unsigned long j = 0; j < server.channels.size(); j++)
		{
			if (server.channels[j].getName() == all_name[i])
			{
				join_chan(client, server, j);
				joined_existing_channel = true;
				break;
			}
		}
		if (!joined_existing_channel)
			create_chan(client, server, all_name[i]);
	}
	return (0);
}
