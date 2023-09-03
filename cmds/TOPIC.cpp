#include "../commands.hpp"
/**
 * Processes the TOPIC command from a client.
 * The TOPIC command is used to query or set the topic of a channel. This function handles:
 * - Checking if the client has provided enough parameters.
 * - Checking if the channel exists.
 * - Querying the current topic of the channel.
 * - Setting a new topic for the channel if the client has the necessary permissions.
 * @param server     Reference to the server object.
 * @param client     Reference to the client object sending the TOPIC command.
 * @param cmd        Vector containing the command details, including the channel name and optional new topic.
 * @return           Returns the number of bytes sent to the client or 0 if no bytes were sent.
 */
int	TOPIC(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::string msg;

	if(cmd.size() < 2 || cmd[1] == "")
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a TOPIC command with not enough parameters." << std::endl;
		return (send(client.getFd(), "461 TOPIC :Not enough parameters\r\n", 35, 0));
	}
	else if (cmd.size() == 2)
	{
		if (server.findChannel(cmd[1]) == -1)
		{
			std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a TOPIC command to a channel that does not exist." << std::endl;
			return (send(client.getFd(), "403 No such channel.\r\n", 23, 0));
		}
		else
		{
			std::string topic = server.channels[server.findChannel(cmd[1])].getTopic();
			if (topic == "")
			{
				std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a TOPIC command but no topic set." << std::endl;
				return (send(client.getFd(), "331 :No topic is set.\r\n", 24, 0));
			}
			msg = ":" + server.getName() + " 332 " + client.getNickname() + " " + cmd[1] + " " + topic + "\r\n";
			return (send(client.getFd(), msg.c_str(), msg.length(), 0));
		}
	}
	else if (cmd.size() > 2)
	{
		std::string NewTopic;
		if (cmd[2][0] != ':')
		{
			std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a TOPIC command with invalid parameters." << std::endl;
			return (send(client.getFd(), "461 TOPIC :Invalid parameters\r\n", 32, 0));
		}
		for (size_t i = 2; i < cmd.size(); i++)
		{
			NewTopic += cmd[i];
			if (i < cmd.size() - 1)
				NewTopic += " ";
		}
		if (server.findChannel(cmd[1]) == -1)
		{
			std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a TOPIC command to a channel that does not exist." << std::endl;
			return (send(client.getFd(), "403 No such channel.\r\n", 23, 0));
		}
		if (server.channels[server.findChannel(cmd[1])].checkFlag('t') == true)
		{
			if (server.channels[server.findChannel(cmd[1])].isOperator(client.getNickname()) == false)
			{
				std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a TOPIC command without chanOP privilege." << std::endl;
				return (send(client.getFd(), "482 You're not an operator.\r\n", 30, 0));
			}
			else
			{
				msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + intToIp(client.getAddr()) + " TOPIC " + cmd[1] + " " + NewTopic + "\r\n";
				server.channels[server.findChannel(cmd[1])].setTopic(NewTopic);
				server.channels[server.findChannel(cmd[1])].sendToChannelMembers(server, msg, "none");
				return (0);
			}
		}
		else
		{
			msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + intToIp(client.getAddr()) + " TOPIC " + cmd[1] + " " + NewTopic + "\r\n";
			server.channels[server.findChannel(cmd[1])].setTopic(NewTopic);
			server.channels[server.findChannel(cmd[1])].sendToChannelMembers(server, msg, "none");
			return (0);
		}
	}
	return (0);
}
