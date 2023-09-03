#include "../commands.hpp"

/**
 * Grants voice privileges to a user in a specific channel.
 * This function constructs a command to modify the user's mode to have voice privileges
 * and then delegates the actual mode change to the MODE function.
 * @param server   Reference to the server object.
 * @param client   Reference to the client object requesting the voice operation.
 * @param cmd      A vector containing the command details. Expected format: ["VOICE", "channel_name", "nickname_to_voice"].
 * @return         Returns the result of the MODE function call.
 */
int VOICE(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> cmd_to_mode;

	cmd_to_mode.push_back("VOICE");
	cmd_to_mode.push_back(cmd[1]);
	cmd_to_mode.push_back("+v");
	cmd_to_mode.push_back(cmd[2]);
	return (MODE(server, client, cmd_to_mode));
}

/**
 * Removes voice privileges from a user in a specific channel.
 * This function constructs a command to modify the user's mode to remove voice privileges
 * and then delegates the actual mode change to the MODE function.
 * @param server   Reference to the server object.
 * @param client   Reference to the client object requesting the devoice operation.
 * @param cmd      A vector containing the command details. Expected format: ["DEVOICE", "channel_name", "nickname_to_devoice"].
 * @return         Returns the result of the MODE function call.
 */
int DEVOICE(Server &server, Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> cmd_to_mode;

	cmd_to_mode.push_back("VOICE");
	cmd_to_mode.push_back(cmd[1]);
	cmd_to_mode.push_back("-v");
	cmd_to_mode.push_back(cmd[2]);
	return (MODE(server, client, cmd_to_mode));
}
