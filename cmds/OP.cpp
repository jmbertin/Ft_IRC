#include "../commands.hpp"

/**
 * Grants operator status to a user in a specified channel.
 * The OP function is a shorthand for the MODE command to grant operator status (+o)
 * to a specified user in a channel. It constructs the appropriate MODE command and
 * delegates the operation to the MODE function.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the operation.
 * @param cmd     Vector containing the command details.
 * @return        Returns the result of the MODE function after processing the command.
 */
int	OP(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> cmd_to_mode;

	cmd_to_mode.push_back("MODE");
	cmd_to_mode.push_back(cmd[1]);
	cmd_to_mode.push_back("+o");
	cmd_to_mode.push_back(cmd[2]);
	return(MODE(server, client, cmd_to_mode));
}

/**
 * Revokes operator status from a user in a specified channel.
 * The DEOP function is a shorthand for the MODE command to revoke operator status (-o)
 * from a specified user in a channel. It constructs the appropriate MODE command and
 * delegates the operation to the MODE function.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the operation.
 * @param cmd     Vector containing the command details.
 * @return        Returns the result of the MODE function after processing the command.
 */
int	DEOP(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> cmd_to_mode;

	cmd_to_mode.push_back("MODE");
	cmd_to_mode.push_back(cmd[1]);
	cmd_to_mode.push_back("-o");
	cmd_to_mode.push_back(cmd[2]);
	return(MODE(server, client, cmd_to_mode));
}
