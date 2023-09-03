#include "../commands.hpp"

/**
 * Processes the PING command from a client.
 * The PING command is used to check the connection between the client and the server.
 * In response to a PING command, the server sends a PONG message back to the client.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object sending the PING command.
 * @param cmd     Vector containing the command details.
 * @return        Returns the number of bytes sent in the PONG response, or -1 if
 *                the send operation fails.
 */
int	PING(Server &server, Client &client, std::vector<std::string> cmd)
{
	(void) cmd;
	std::string pong = "000 PONG " + server.getName() + "\r\n";
	return (send(client.getFd(), pong.c_str(), pong.length(), 0));
}
