#include "../commands.hpp"

/**
 * Processes the PASS command for client authentication.
 * The PASS command is used by a client to set a connection password. If the client
 * is already authenticated, an error message is returned indicating that the client
 * is already registered.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the password.
 * @param cmd     Vector containing the command details.
 * @return        Returns 0 if the operation is successful, or ERR_ALREADYREGISTRED
 *                if the client is already registered.
 */
int	PASS(Server &server, Client &client, std::vector<std::string> cmd)
{
	(void) server;
	(void) cmd;
	if (client.getAuth())
	{
		send(client.getFd(), "462 : You are already registered\r\n", 35, 0);
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried PASS while already registered." << std::endl;
		return (ERR_ALREADYREGISTRED);
	}
	return (0);
}
