#include "../commands.hpp"

/**
 * Checks if a character is considered special for the context of nicknames.
 * @param c  Character to be checked.
 * @return   Returns true if the character is special, false otherwise.
 */
static bool	isSpecial(char c)
{
	return (c == '[' || c == ']' || c == '\\' || c == '`' ||
		c == '^' || c == '{' || c == '}' || c == '|' || c == '_');
}

/**
 * Validates a given nickname based on specific criteria.
 * @param nickname  The nickname string to be validated.
 * @return          Returns true if the nickname is valid, false otherwise.
 */
static bool	isValidNickname(const std::string& nickname)
{
	if (nickname.empty() || nickname.size() > 9)
		return false;
	if (!std::isalpha(static_cast<unsigned char>(nickname[0])) && !isSpecial(nickname[0]))
		return false;
	for (std::string::size_type i = 1; i < nickname.size(); i++)
	{
		char c = nickname[i];
		if (!((std::isalnum(static_cast<unsigned char>(c)) || c == '-' || isSpecial(c)) &&
				(c != '\0' && c != '\n' && c != '\r' && c != ' ')))
			return false;
	}
	return true;
}

/**
 * Handles the NICK command to set or change a client's nickname.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set/change the nickname.
 * @param line    Vector containing the command details.
 * @return        Returns ERR_NOSUCHNICK if no nickname is given, ERR_ERRONEUSNICKNAME if the nickname format is invalid,
 *                ERR_NICKNAMEINUSE if the nickname is already in use, and 1 if the nickname is successfully set/changed.
 */
int	NICK(Server &server, Client &client, std::vector<std::string> line)
{
	if (line.size() < 2)
	{
		send(client.getFd(), "431 : No nickname given\r\n", 26, 0);
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried command NICK without nickname." << std::endl;
		return (ERR_NOSUCHNICK);
	}
	if (!isValidNickname(line[1]))
	{
		send(client.getFd(), "432 : Erroneous nickname (format : 1* letter/special) + *8 letter/digit/special([]\\_`^{}|)))\r\n", 95, 0);
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to set unautorized nickname(" << line[1] << ")." << std::endl;
		return (ERR_ERRONEUSNICKNAME);
	}
	for (std::vector<Client*>::const_iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
	{
		if ((*it)->getNickname() == line[1])
		{
			send(client.getFd(), "433 : Nickname is already in use\r\n", 35, 0);
			std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to set a nickname already in use (" << line[1] << ")." << std::endl;
			return (ERR_NICKNAMEINUSE);
		}
	}
	std::string newNickname = line[1];
	std::string oldNickname = client.getNickname();
	client.setNickname(newNickname);
	std::cout << "** INFO : " << intToIp(client.getAddr()) << " changed nickname to " << client.getNickname() << "." << std::endl;
	return (1);
}
