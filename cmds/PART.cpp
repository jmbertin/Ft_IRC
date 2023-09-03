#include "../commands.hpp"

/**
 * Allows a user to leave one or more channels.
 * The PART command is used by a client to leave one or more channels. When a client
 * leaves a channel, a PART message is sent to all members of the channel to notify
 * them of the client's departure. If the client is not a member of the channel, an
 * error message is returned to the client.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the departure.
 * @param param   Vector containing the command details, including channel names and
 *                an optional quit message.
 * @return        Returns 0 if the operation is successful, or an error code if an
 *                error occurs.
 */
int	PART(Server &server, Client &client, std::vector <std::string> param)
{
	std::string quitMessage;

	if (param.size() < 3)
		quitMessage = "Client Quit";
	else
	{
		if (param[2][0] == ':')
			quitMessage = &param[2][1];
		else
			quitMessage = param[2];
		for (size_t i = 3; i < param.size(); ++i)
		{
			quitMessage += " " + param[i];
		}
	}
	std::string formattedQuitMessage = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " PART ";

	if (param.size() < 2)
	{
		send(client.getFd(), "461 PART :Not enough parameters\r\n", 32, 0);
		return (ERR_NEEDMOREPARAMS);
	}
	std::vector<std::string> channelNames = splitString(param[1], ',');
	for (std::vector<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); ++it)
	{
		int chan_index = server.findChannel(*it);
		if (chan_index != -1)
		{
			if (server.channels[chan_index].isMember(client.getNickname()))
			{
				formattedQuitMessage += *it + " :" + quitMessage + "\r\n";
				server.channels[chan_index].sendToChannelMembers(server, formattedQuitMessage, "NONE");
				server.channels[chan_index].removeMember(client.getNickname());
				std::cout << "** INFO : " << intToIp(client.getAddr()) << " left channel " << *it << "." << std::endl;
			}
			else
			{
				std::string priverror = ":" + server.getName() + " 442 " + client.getNickname() + " " + *it + " :You're not on this channel\r\n";
				send(client.getFd(), priverror.c_str(), priverror.length(), 0);
				std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to left channel " << *it << " without being a member." << std::endl;
			}
		}
		else
		{
			std::string priverror = ":" + server.getName() + " 403 " + client.getNickname() + " " + *it + " :No such channel\r\n";
			send(client.getFd(), priverror.c_str(), priverror.length(), 0);
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to left channel " << *it << " which doesn't exist." << std::endl;
		}
	}
	return (0);
}
