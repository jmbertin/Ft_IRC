#include "../commands.hpp"

/**
 * Handles the NOTICE command to send a notice message to a specified target.
 * The NOTICE command is used to send a message to a specified target (either a user or a channel)
 * without expecting a reply. This function processes the NOTICE command, validates the target and
 * message, and sends the notice to the appropriate recipient(s).
 * @param server  Reference to the server object.
 * @param client  Reference to the client object sending the notice.
 * @param line    Vector containing the command details.
 * @return        Returns 0 after processing the NOTICE command.
 */
int	NOTICE(Server &server, Client &client, std::vector<std::string> line)
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

	if (line.size() < 3)
	{
		if (line[1][0] == ':')
		{
			std::string priverror = ":" + server.getName() + " 411 " + client.getNickname() + " :No recipient given (NOTICE)\r\n";
			send(client.getFd(), priverror.c_str(), priverror.length(), 0);
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a notice without a target." << std::endl;
		}
		else
		{
			std::string priverror = ":" + server.getName() + " 412 " + client.getNickname() + " :No text to send\r\n";
			send(client.getFd(), priverror.c_str(), priverror.length(), 0);
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a notice without a message." << std::endl;
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
				std::string message = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " NOTICE " + *it + " :" + messageText + "\r\n";
				server.channels[chan_index].sendToChannelMembers(server, message, client.getNickname());
				std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a notice to channel " << *it << "." << std::endl;
			}
			else
			{
				std::string priverror = ":" + server.getName() + " 403 " + client.getNickname() + " " + *it + " :No such channel\r\n";
				send(client.getFd(), priverror.c_str(), priverror.length(), 0);
				std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a notice to non-existent channel " << *it << "." << std::endl;
			}
		}
		else
		{
			Client *dest = server.getClientByNickname(*it);
			if (dest)
			{
				std::string message = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " NOTICE " + *it + " :" + messageText + "\r\n";
				send(dest->getFd(), message.c_str(), message.length(), 0);
				std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a notice to user " << *it << "." << std::endl;
			}
			else
			{
				std::string priverror = ":" + server.getName() + " 401 " + client.getNickname() + " " + *it + " :No such nick/channel\r\n";
				send(client.getFd(), priverror.c_str(), priverror.length(), 0);
				std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to send a notice to non-existent user " << *it << "." << std::endl;
			}
		}
	}
	return (0);
}
