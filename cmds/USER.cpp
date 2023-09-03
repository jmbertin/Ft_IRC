#include "../commands.hpp"

/**
 * Checks if a character is considered special based on a predefined set of characters.
 *
 * @param c    The character to check.
 * @return     Returns true if the character is special, otherwise returns false.
 */
static bool	isSpecial(char c)
{
	return (c == '[' || c == ']' || c == '\\' || c == '`' ||
		c == '^' || c == '{' || c == '}' || c == '|' || c == '_');
}

/**
 * Validates a given username based on specific criteria.
 * A valid username:
 * - Should not be empty.
 * - Should not exceed 10 characters.
 * - Can only contain alphanumeric characters, hyphens, or special characters defined by the isSpecial function.
 * @param username    The username string to validate.
 * @return            Returns true if the username is valid, otherwise returns false.
 */
static bool	isValidUsername(const std::string& username)
{
	if (username.empty() || username.size() > 10)
		return false;
	for (std::string::size_type i = 0; i < username.size(); i++)
	{
		if (!std::isalnum(static_cast<unsigned char>(username[i])) && username[i] != '-' && !isSpecial(username[i]))
			return false;
	}
	return true;
}

/**
 * Validates a given realname based on specific criteria.
 * A valid realname:
 * - Should not be empty.
 * - Should start with a colon ':'.
 * - Should not exceed 50 characters.
 * - Should not contain null ('\0'), newline ('\n'), or carriage return ('\r') characters.
 * @param realname    The realname string to validate.
 * @return            Returns true if the realname is valid, otherwise returns false.
 */
static bool isValidRealname(const std::string& realname)
{
	if (realname.empty() || realname[0] != ':' || realname.length() > 50)
		return false;
	for (std::string::const_iterator it = realname.begin(); it != realname.end(); ++it)
	{
		if (*it == '\0' || *it == '\n' || *it == '\r')
			return false;
	}
	return true;
}

/**
 * Processes the USER command from a client.
 *
 * The USER command is used to set the username and realname for a client. This function handles:
 * - Checking if the client is already authenticated.
 * - Validating the provided username and realname.
 * - Setting the username and realname for the client if they are valid.
 * - Handling errors and sending appropriate error messages to the client.
 *
 * @param server     Reference to the server object.
 * @param client     Reference to the client object sending the USER command.
 * @param splitted   Vector containing the command details, including the username and realname.
 * @return           Returns an error code or 0 if no error occurred. Returns 1 if the username and realname were set successfully.
 */
int	USER(Server &server, Client &client, std::vector<std::string> splitted)
{
	if (client.getAuth())
	{
		send(client.getFd(), "462 : You are already registered\r\n", 35, 0);
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried USER while already registered." << std::endl;
		return (ERR_ALREADYREGISTRED);
	}
	if (splitted.size() < 2)
	{
		send(client.getFd(), "431 : No username given\r\n", 26, 0);
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried USER without username." << std::endl;
		return (ERR_NOSUCHNICK);
	}
	for (std::vector<Client*>::const_iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
	{
		if ((*it)->getUsername() == splitted[1])
		{
			send(client.getFd(), "433 : Username is already in use\r\n", 35, 0);
			std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to set a username already in use (" << splitted[1] << ")." << std::endl;
			return (0);
		}
	}
	if (isValidUsername(splitted[1]))
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " set username to " << splitted[1] << "." << std::endl;
		client.setUsername(splitted[1]);
	}
	else
	{
		send(client.getFd(), "432 : Erroneous username (format : *9 letter/digit/special([]\\_`^{}|))\r\n", 73, 0);
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to set unautorized username(" << splitted[1] << ")." << std::endl;
		return (ERR_ERRONEUSNICKNAME);
	}
	std::string realname;
	if (splitted.size() >= 5 && splitted[4].size() > 1)
		realname = splitted[4];
	if (splitted.size() >= 6)
	{
		for (size_t i = 5; i < splitted.size(); ++i)
		{
			if (i < splitted.size())
				realname += " ";
			realname += splitted[i];
		}
	}
	if (isValidRealname(realname))
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " set realname to " << &realname[1] << "." << std::endl;
		client.setRealname(&realname[1]);
		return (1);
	}
	else
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " tried to set realname to " << &realname[1] << " (unautorized, set to Anonymous User)." << std::endl;
		client.setRealname("Anonymous user");
		return (1);
	}
	return (0);
}
