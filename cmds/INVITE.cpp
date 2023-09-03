#include "../commands.hpp"

/**
 * Invites a user to join a specific channel.
 * The function checks if the command has the correct number of parameters.
 * If the client is an operator of the channel, they can invite another user.
 * If the user is already a member or has been invited, an error is returned.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the invite.
 * @param param     Vector containing the command details. Expected format: ["INVITE", "target_nickname", "channel_name"].
 * @return          Returns 0 if the invite is successful, or an error code corresponding to the type of error encountered.
 */
int	INVITE(Server &server, Client &client, std::vector<std::string> param)
{
	if (param.size() < 3)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a INVITE command with not enough parameters." << std::endl;
		return (send(client.getFd(), "461 INVITE :Not enough parameters\r\n", 36, 0));
	}
	else if (param.size() > 3)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a INVITE command with too many parameters." << std::endl;
		return (send(client.getFd(), "461 INVITE :Too many parameters\r\n", 34, 0));
	}
	else
	{
		// <your_nickname> INVITE <target_nickname> :<channel>
		std::string	invite = client.getNickname() + " INVITE " + param[1] + " :" + param[2] + "\r\n";
		int chan_index = server.findChannel(param[2]);
		if (chan_index != -1)
		{
			if (server.channels[chan_index].isOperator(client.getNickname()))
			{
				if (server.channels[chan_index].isMember(param[1]))
				{
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " sent a INVITE command to " << param[1] << " to join " << param[2] << " (allready in channel)." << std::endl;
					send(client.getFd(), "443 User is allready on channel.\r\n", 35, 0);
					return(ERR_USERONCHANNEL);
				}
				else
				{
					if (server.channels[chan_index].checkInvited(param[1]))
					{
						std::cout << "** ERROR : " << intToIp(client.getAddr()) << " sent a INVITE command to " << param[1] << " to join " << param[2] << " (allready invited)." << std::endl;
						send(client.getFd(), "443 User is allready invited.\r\n", 32, 0);
						return(ERR_USERONCHANNEL);
					}
					Client *dest = server.getClientByNickname(param[1]);
					if (dest)
					{
						server.channels[chan_index].addInvited(param[1]);
						std::cout << "** INFO : " << intToIp(client.getAddr()) << " sent a INVITE command to " << param[1] << " to join " << param[2] << "." << std::endl;
						send(client.getFd(), invite.c_str(), invite.length(), 0);
						send(dest->getFd(), invite.c_str(), invite.length(), 0);
						return (0);
					}
					else
					{
						std::cout << "** ERROR : " << intToIp(client.getAddr()) << " sent a INVITE command to " << param[1] << " to join " << param[2] << " (no such nick)." << std::endl;
						send(client.getFd(), "401 No such nick.\r\n", 20, 0);
						return(ERR_NOSUCHNICK);
					}
				}
			}
			else
			{
				std::cout << "** ERROR : " << intToIp(client.getAddr()) << " sent a INVITE command to " << param[1] << " to join " << param[2] << " (not operator)." << std::endl;
				send(client.getFd(), "482 You're not an operator.\r\n", 30, 0);
				return(ERR_CHANOPRIVSNEEDED);
			}
		}
		else
		{
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " sent a INVITE command to " << param[1] << " to join " << param[2] << " (no such channel)." << std::endl;
			send(client.getFd(), "403 No such channel.\r\n", 23, 0);
			return(ERR_NOSUCHCHANNEL);
		}
	}
}
