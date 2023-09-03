#include "../commands.hpp"

/**
 * Handles the IRC WHO command.
 * This function processes the WHO command, which is used to obtain a list of users in a channel or information about a specific user.
 * If the target is a channel, it retrieves the list of members in the channel and sends their details to the requesting client.
 * If the target is a nickname, it retrieves the details of the specified user and sends them to the requesting client.
 * @param server    Reference to the server object.
 * @param client    Reference to the client object that sent the WHO command.
 * @param arguments A vector containing the command details. Expected format: ["WHO", "target"], where "target" can be a channel name or a nickname.
 * @return          Returns 1 if there's an error (e.g., not enough parameters), otherwise returns 0.
 */
int	WHO(Server &server, Client &client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		std::string error = ":" + server.getName() + " 461 " + client.getNickname() + " WHO :Not enough parameters\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to use WHO command without parameters." << std::endl;
		return (1);
	}

	std::string target = arguments[1];
	if (target[0] == '#')
	{
		int chan_index = server.findChannel(target);
		if (chan_index != -1)
		{
			const std::vector<std::string> &members = server.channels[chan_index].getClients();
			for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end(); ++it)
			{
				Client *member = server.getClientByNickname(*it);
				if (member != NULL)
				{
					std::string response = ":" + server.getName() + " 352 " + client.getNickname() + " " + target + " " + member->getUsername() + " " + intToIp(member->getAddr()) + " " + server.getName() + " " + member->getNickname() + " H :0 " + member->getRealname() + "\r\n";
					send(client.getFd(), response.c_str(), response.length(), 0);
				}
			}
			if (server.channels[server.findChannel(target)].getBot() != NULL)
			{
				Client *bot = server.channels[server.findChannel(target)].getBot();
				std::string response = ":" + server.getName() + " 352 " + client.getNickname() + " " + target + " " + bot->getUsername() + " " + intToIp(bot->getAddr()) + " " + server.getName() + " " + bot->getNickname() + " H :0 " + bot->getRealname() + "\r\n";
				send(client.getFd(), response.c_str(), response.length(), 0);
			}
			std::string end_response = ":" + server.getName() + " 315 " + client.getNickname() + " " + target + " :End of WHO list\r\n";
			send(client.getFd(), end_response.c_str(), end_response.length(), 0);
		}
	}
	else
	{
		Client *foundClient = server.getClientByNickname(target);
		if (foundClient != NULL)
		{
			std::string response = ":" + server.getName() + " 352 " + client.getNickname() + " * " + foundClient->getUsername() + " " + intToIp(foundClient->getAddr()) + " " + server.getName() + " " + foundClient->getNickname() + " H :0 " + foundClient->getRealname() + "\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			std::string end_response = ":" + server.getName() + " 315 " + client.getNickname() + " " + target + " :End of WHO list\r\n";
			send(client.getFd(), end_response.c_str(), end_response.length(), 0);
		}
	}

	return (0);
}

