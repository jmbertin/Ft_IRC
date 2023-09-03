#include "../commands.hpp"

/**
 * Processes and executes capability-related commands.
 * If the command is "LS", it sends a list of capabilities of the server.
 * If the command is "REQ", it sends a negative acknowledgment for the requested capabilities.
 * @param arguments  Vector containing the command details.
 * @param server     Reference to the server object where the action will take place.
 * @param client     Reference to the client object initiating the command.
 * @return           Returns 0 after execution.
 */
int CAP(Server &server, Client &client, std::vector<std::string> arguments)
{
	if (arguments.size() >= 2 && arguments[1] == "LS")
	{
		std::string msg = ":" + server.getName() + " LS :\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
	}
	else if (arguments.size() >= 3 && arguments[1] == "REQ")
	{
		std::string requestedFeatures = arguments[2].substr(1);
		for (size_t i = 3; i < arguments.size(); ++i)
		{
			requestedFeatures += " " + arguments[i];
		}
		std::string capMessage = ":" + server.getName() + " CAP * NAK :" + requestedFeatures + "\r\n";
		send(client.getFd(), capMessage.c_str(), capMessage.size(), 0);
	}
	return (0);
}
