#include "commands.hpp"

/**
 * Checks if the provided password meets the required criteria.
 * @param password The password string to be checked.
 * @return Returns 1 if the password is valid, otherwise returns 0.
 */
int	ft_check_password(std::string password)
{
	if (password.length() < 4 || password.length() > 10)
		return (0);
	for(size_t i = 0; i < password.length(); i++)
	{
		if (!isalnum(password[i]))
			return (0);
	}
	return (1);
}

/**
 * Checks if the provided port meets the required criteria.
 * @param port The port string to be checked.
 * @return Returns 1 if the port is valid, otherwise returns 0.
 */
int	ft_check_port(std::string port)
{
	if (port.length() != 4)
		return (0);
	else if (port[0] != '6' && port[1] != '6' && port[2] != '6')
		return (0);
	else if (port[3] != '5' && port[3] != '6' && port[3] != '7' && port[3] != '8' && port[3] != '9')
		return (0);
	return (1);
}

/**
 * The main function for the IRC server application.
 * @param ac The number of command-line arguments.
 * @param av The array of command-line arguments.
 * @return Returns 0 on successful execution, otherwise returns -1.
 */
int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Bad arguments numbers, should be 2 : ./ft_irc <port> <password>" << std::endl;
		return -1;
	}
	else if (!ft_check_port(av[1]))
	{
		std::cerr << "Bad IRC port should be in range 6665-6669." << std::endl;
		return -1;
	}
	else if (!ft_check_password(av[2]))
	{
		std::cerr << "Bad IRC password, size should be 4 to 10 characters (alphanum only)." << std::endl;
		return -1;
	}

	Server *server = new Server(av[1], av[2]);
	server->mainLoop();
	delete server;
	return (0);
}
