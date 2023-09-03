#include "../commands.hpp"

/**
 * Shuts down the server.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the command.
 * @param param     Vector containing the command details (not used in this function).
 * @return          Returns 0 after execution.
 */
int	EXITS(Server &server, Client &client, std::vector<std::string> param)
{
	(void)param;
	(void)client;
	(void)server;
	server.setOff();
	return (0);
}
