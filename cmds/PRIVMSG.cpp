#include "../commands.hpp"
#include <stdlib.h>

/**
 * Processes the PRIVMSG command from a client.
 * The PRIVMSG command is used by clients to send private messages to other clients or channels.
 * This function handles the sending of the message, checks for valid recipients, and ensures
 * that the client has the necessary permissions to send the message to the specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object sending the PRIVMSG command.
 * @param line    Vector containing the command details.
 * @return        Returns 0 if the message is processed successfully, and 1 if there are errors
 *                or if the client is kicked by a bot for violations like spamming.
 */
int	PRIVMSG(Server &server, Client &client, std::vector<std::string> line)
{
	if (line.size() > 2)
	{
		if (line[2][0] == ':')
			line[2].erase(0, 1);
	}
	std::string messageText;
	for (size_t i = 2; i < line.size(); i++)
	{
		messageText += line[i];
		if (i < line.size() - 1)
			messageText += " ";
	}
	if (line.size() == 1)
	{
		std::string priverror = ":" + server.getName() + " 412 " + client.getNickname() + " :No text to send\r\n";
		send(client.getFd(), priverror.c_str(), priverror.length(), 0);
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message without a message." << std::endl;
		return (0);
	}
	if (line.size() == 2)
	{
		if (line[1][0] == ':')
		{
			std::string priverror = ":" + server.getName() + " 411 " + client.getNickname() + " :No recipient given (PRIVMSG)\r\n";
			send(client.getFd(), priverror.c_str(), priverror.length(), 0);
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message without a target." << std::endl;
		}
		else
		{
			std::string priverror = ":" + server.getName() + " 412 " + client.getNickname() + " :No text to send\r\n";
			send(client.getFd(), priverror.c_str(), priverror.length(), 0);
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message without a message." << std::endl;
		}
		return (0);
	}

	std::vector<std::string> channelNames = splitString(line[1], ',');
	for (std::vector<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); ++it)
	{
		if ((*it)[0] == '#')
		{
			int chan_index = server.findChannel(*it);
			if (chan_index != -1)
			{
				if (server.channels[chan_index].checkFlag('m') == true && (server.channels[chan_index].isVoiced(client.getNickname()) == false && server.channels[chan_index].isOperator(client.getNickname()) == false))
				{
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message to channel " << *it << " without being voiced." << std::endl;
					std::string priverror = ":" + server.getName() + " 404 " + client.getNickname() + " " + *it + " :Cannot send to channel\r\n";
					send(client.getFd(), priverror.c_str(), priverror.length(), 0);
					return (1);
				}
				else if (server.channels[chan_index].isMember(client.getNickname()))
				{
					struct timeval currentTime;
					gettimeofday(&currentTime, NULL);
					if (server.channels[chan_index].getBot() != NULL)
					{
						if (checkGrossBot(line))
						{
							std::cout << "** WARNING : GROSS DETECTED FROM " << intToIp(client.getAddr()) << " ON CHANNEL " << *it << "." << std::endl;
							kickByBot(server, client, chan_index, "GROSS DETECTED");
							return (0);
						}
						if (currentTime.tv_sec - client.getLastMessageTime().tv_sec < 3)
						{
							client.setMessageCount(client.getMessageCount() + 1);
							if (client.getMessageCount() > 3)
							{
								std::cout << "** WARNING : SPAM DETECTED FROM " << intToIp(client.getAddr()) << " ON CHANNEL " << *it << "." << std::endl;
								kickByBot(server, client, chan_index, "SPAM DETECTED");
								return (0);
							}
						}
					}
					else
					{
						client.setMessageCount(1);
						client.setLastMessageTime(currentTime);
					}
					std::string message = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " PRIVMSG " + *it + " :" + messageText + "\r\n";
					server.channels[chan_index].sendToChannelMembers(server, message, client.getNickname());
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a message to channel " << *it << "." << std::endl;
				}
				else
				{
					std::string priverror = ":" + server.getName() + " 404 " + client.getNickname() + " " + *it + " :Cannot send to channel\r\n";
					send(client.getFd(), priverror.c_str(), priverror.length(), 0);
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message to channel " << *it << " without being a member." << std::endl;
				}
			}
			else
			{
				std::string priverror = ":" + server.getName() + " 403 " + client.getNickname() + " " + *it + " :No such channel\r\n";
				send(client.getFd(), priverror.c_str(), priverror.length(), 0);
				std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message to non-existent channel " << *it << "." << std::endl;
			}
		}
		else
		{
			Client *dest = server.getClientByNickname(*it);
			if (dest)
			{
				std::string message = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " PRIVMSG " + *it + " :" + messageText + "\r\n";
				send(dest->getFd(), message.c_str(), message.length(), 0);
				std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a message to user " << *it << "." << std::endl;
			}
			else
			{
				std::string priverror = ":" + server.getName() + " 401 " + client.getNickname() + " " + *it + " :No such nick/channel\r\n";
				send(client.getFd(), priverror.c_str(), priverror.length(), 0);
				std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a message to non-existent user " << *it << "." << std::endl;
			}
		}
	}
	return (0);
}
