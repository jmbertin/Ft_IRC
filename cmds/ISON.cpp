#include "../commands.hpp"

/**
 * Checks if the specified nicknames are currently online on the server.
 * The function returns a list of nicknames that are currently online from the provided list.
 * If no nicknames are provided, an error message is returned indicating not enough parameters.
 * @param server     Reference to the server object where the action will take place.
 * @param client     Reference to the client object initiating the command.
 * @param arguments  Vector containing the command details. The first element is "ISON" followed by a list of nicknames to check.
 * @return           Returns 0 if the command is processed successfully, or 1 if there are not enough parameters.
 */

int ISON(Server &server, Client &client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		std::string error = ":" + server.getName() + " 461 " + client.getNickname() + " ISON :Not enough parameters\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to use ISON command without parameters." << std::endl;
		return (1);
	}

	std::string response = ":" + server.getName() + " 303 " + client.getNickname() + " :";
	bool isFirstNickname = true;

	for (size_t i = 1; i < arguments.size(); ++i)
	{
		Client *foundClient = server.getClientByNickname(arguments[i]);
		if (foundClient != NULL)
		{
			if (!isFirstNickname)
				response += " ";
			response += foundClient->getNickname();
			isFirstNickname = false;
		}
	}
	response += "\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
	return (0);
}
