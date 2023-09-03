#include "../commands.hpp"

/**
 * Bans a specific user from a server.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the ban.
 * @param cmd       Vector containing the command details. Expected format: ["BAN", "target_channel", "target_user"].
 * @return          Returns the result of the MODE function after executing the ban command.
 */
int BAN(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> cmd_to_mode;

	cmd_to_mode.push_back("BAN");
	cmd_to_mode.push_back(cmd[1]);
	cmd_to_mode.push_back("+b");
	cmd_to_mode.push_back(cmd[2]);
	return(MODE(server, client, cmd_to_mode));
}

/**
 * Unbans a specific user from a server.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the unban.
 * @param cmd       Vector containing the command details. Expected format: ["UNBAN", "target_channel", "target_user"].
 * @return          Returns the result of the MODE function after executing the unban command.
 */

int UNBAN(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> cmd_to_mode;

	cmd_to_mode.push_back("UNBAN");
	cmd_to_mode.push_back(cmd[1]);
	cmd_to_mode.push_back("-b");
	cmd_to_mode.push_back(cmd[2]);
	return(MODE(server, client, cmd_to_mode));
}
