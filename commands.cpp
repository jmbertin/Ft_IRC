#include "commands.hpp"

/**
 * Type definition for a command function.
 * @param server The server instance.
 * @param client The client instance.
 * @param args The arguments provided to the command.
 * @return An integer indicating the result of the command execution.
 */
typedef int (*CommandFunction)(Server&, Client&, std::vector<std::string>);
std::map<std::string, CommandFunction> initializeCommandMap()
{
	std::map<std::string, CommandFunction> commandMap;
	commandMap.insert(std::make_pair("BAN", BAN));
	commandMap.insert(std::make_pair("BOT", BOT));
	commandMap.insert(std::make_pair("CAP", CAP));
	commandMap.insert(std::make_pair("DEOP", DEOP));
	commandMap.insert(std::make_pair("DEVOICE", DEVOICE));
	commandMap.insert(std::make_pair("INVITE", INVITE));
	commandMap.insert(std::make_pair("ISON", ISON));
	commandMap.insert(std::make_pair("JOIN", JOIN));
	commandMap.insert(std::make_pair("KICK", KICK));
	commandMap.insert(std::make_pair("MODE", MODE));
	commandMap.insert(std::make_pair("NICK", NICK));
	commandMap.insert(std::make_pair("NOTICE", NOTICE));
	commandMap.insert(std::make_pair("OP", OP));
	commandMap.insert(std::make_pair("PART", PART));
	commandMap.insert(std::make_pair("PASS", PASS));
	commandMap.insert(std::make_pair("PING", PING));
	commandMap.insert(std::make_pair("PRIVMSG", PRIVMSG));
	commandMap.insert(std::make_pair("QUIT", QUIT));
	commandMap.insert(std::make_pair("TOPIC", TOPIC));
	commandMap.insert(std::make_pair("UNBAN", UNBAN));
	commandMap.insert(std::make_pair("USER", USER));
	commandMap.insert(std::make_pair("VOICE", VOICE));
	commandMap.insert(std::make_pair("WHO", WHO));
	commandMap.insert(std::make_pair("EXITS", EXITS));

	return (commandMap);
}

/**
 * Executes the appropriate command based on the provided parameters.
 * @param param The command parameters.
 * @param client The client instance.
 * @param server The server instance.
 * @return An integer indicating the result of the command execution.
 */
int switchCmd(std::vector<std::string> param, Client &client, Server &server)
{
	static const std::map<std::string, CommandFunction> commandMap = initializeCommandMap();

	std::map<std::string, CommandFunction>::const_iterator commandIt = commandMap.find(param[0]);
	if (commandIt != commandMap.end())
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to " << param[0] << "." << std::endl;
		return commandIt->second(server, client, param);
	}
	else
	{
		std::string unknow_cmd = "421 " + param[0] + " :Unknown command\r\n";
		return (send(client.getFd(), unknow_cmd.c_str(), unknow_cmd.length(), 0));
	}
}
