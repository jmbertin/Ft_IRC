#include "../commands.hpp"

/**
 * Performs the actual kick operation on a target user from a specified channel.
 * This function checks if the channel exists, if the target user is in the channel, and if the client has the operator privilege.
 * If all checks pass, the target user is kicked from the channel.
 * @param client      Reference to the client object initiating the kick.
 * @param server      Reference to the server object.
 * @param channel_id  Index of the channel in the server's channel list.
 * @param target      Nickname of the user to be kicked.
 * @param opt_message Optional message to be sent with the kick.
 * @return            Returns 0 if the kick is successful, or an error code corresponding to the type of error encountered.
 */
int	actual_kick(Client &client, Server &server, int channel_id, std::string target, std::string opt_message)
{
	if (channel_id == -1)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a KICK command to a channel that does not exist." << std::endl;
		return (send(client.getFd(), "403 No such channel.\r\n", 22, 0));
	}
	if (server.channels[channel_id].isIn(target) != true)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a KICK command to a user that does not exist." << std::endl;
		return (send(client.getFd(), "441 User is not in this channel.\r\n", 34, 0));
	}
	if (server.channels[channel_id].isOperator(client.getNickname()) != true)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a KICK command without chanOP privilege." << std::endl;
		return (send(client.getFd(), "482 You're not an operator.\r\n", 29, 0));
	}

	server.channels[channel_id].kick(target);
	std::string msg = ":" + server.getName() + " KICK " + server.channels[channel_id].getName() + " " + target + " ";
	if (opt_message != "")
		msg += opt_message + "\r\n";
	else
		msg += ":" + client.getNickname() + "\r\n";
	Client *target_client = server.getClientByNickname(target);
	server.channels[channel_id].sendToChannelMembers(server, msg, "none");
	send(target_client->getFd(), msg.c_str(), msg.length(), 0);
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	return (0);
}

/**
 * Processes the KICK command to kick one or more users from one or more channels.
 * The function checks for the correct number of parameters and then delegates the actual kick operation to the `actual_kick` function.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the kick.
 * @param cmd     Vector containing the command details.
 * @return        Returns 0 if the command is processed successfully, or an error code corresponding to the type of error encountered.
 */
int	KICK(Server &server, Client &client, std::vector<std::string> cmd)
{
	if ((cmd.size() != 3 && cmd.size() != 4) || cmd[2] == "BOT")
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a KICK command with not enough parameters." << std::endl;
		return (send(client.getFd(), "461 KICK :Not enough parameters\r\n", 33, 0));
	}

	std::vector<std::string> all_channels = splitString(cmd[1], ',');
	std::vector<std::string> all_users = splitString(cmd[2], ',');
	std::string opt_message = "";
	if (cmd.size() == 4)
		opt_message = cmd[3];

	if (all_users.size() != all_channels.size() && all_channels.size() != 1)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a KICK command with bad syntax." << std::endl;
		return (send(client.getFd(), "461 KICK :Not enough parameters\r\n", 33, 0));
	}
	if (all_channels.size() != 1)
	{
		for (unsigned int i = 0; i < all_channels.size(); i++)
		{
			int	channel_id = server.findChannel(all_channels[i]);
			actual_kick(client, server, channel_id, all_users[i], opt_message);
		}
	}
	else
	{
		int channel_id = server.findChannel(all_channels[0]);
		for (unsigned int i = 0; i < all_users.size(); i++)
			actual_kick(client, server, channel_id, all_users[i], opt_message);
	}
	return (0);
}
