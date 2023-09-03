#include "Server.hpp"

/**
 * Destructor for the Server class.
 * Cleans up allocated resources and notifies about the destruction.
 */
Server::~Server()
{
	std::cout << "Server destructor called" << std::endl;

	for (std::vector<Channel>::iterator channel_it = channels.begin(); channel_it != channels.end(); ++channel_it)
	{
		Channel* channel = &(*channel_it);
		Client* bot = channel->getBot();
		if (bot)
		{
			delete bot;
			channel->setBot(NULL);
		}
	}

	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		delete *it;
	}
}

/**
 * Constructor for the Server class.
 * Initializes the server with the provided port and password, sets up the socket and binds it.
 * @param port The port number as a string on which the server should listen.
 * @param password The password required for clients to authenticate.
 */
Server::Server(std::string port, std::string password) : _password(password), _name("ft_irc"), _isOn(true)
{
	int	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	pollfd listen_pollfd;
	listen_pollfd.fd = listen_fd;
	listen_pollfd.events = POLLIN;
	this->_pollfds.push_back(listen_pollfd);

	if (listen_fd == -1)
	{
		perror("Socket creation error.");
		exit(1);
	}

	fcntl(listen_fd, F_SETFL, O_NONBLOCK);

	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(atoi(port.c_str()));
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listen_fd, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) == -1)
	{
		perror("Linking socket error.");
		exit(1);
	}

	if (listen(listen_fd, SOMAXCONN) == -1)
	{
		perror("Error while listening socket.");
		exit(1);
	}

	this->_commands["NICK"] = &NICK;
	this->_commands["USER"] = &USER;
}

/**
 * Retrieves the list of connected clients.
 * @return A constant reference to the vector containing pointers to connected clients.
 */
const std::vector<Client*>&	Server::getClients(void) const { return (this->_clients); }

/**
 * Retrieves the file descriptor of the listening socket.
 * @return The file descriptor of the listening socket.
 */
int	Server::getListenFd(void) const { return (this->_pollfds[0].fd); }

/**
 * Retrieves the name of the server.
 * @return A constant reference to the server's name string.
 */
const std::string&	Server::getName() const { return (this->_name); }

/**
 * The main loop of the server where it listens for incoming connections and handles client requests.
 * Continues to run as long as the server is active.
 */
void	Server::mainLoop()
{
	while (this->_isOn)
	{
		int ret = poll(_pollfds.data(), _pollfds.size(), -1);
		if (ret == -1)
		{
			perror("Poll error.");
			exit(1);
		}

		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (!(_pollfds[i].revents & POLLIN))
				 continue;
			if (_pollfds[i].fd == getListenFd())
				acceptClientConnection();
			else
				handleClient(i);
		}
	}
	close(getListenFd());
}

/**
 * Accepts a new client connection, sets up the necessary file descriptors, and adds the client to the list.
 */
void	Server::acceptClientConnection()
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	int client_fd = accept(getListenFd(), (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd == -1)
	{
		perror("Client connection error.");
		exit(1);
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	_pollfds.push_back(client_pollfd);

	char client_address[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_address, INET_ADDRSTRLEN);
	_clients.push_back(new Client(client_fd, client_addr));

	std::cout << "-> "<< client_address << " : Connexion attempt." << std::endl;
}

/**
 * Handles incoming data from a client, processes the received data, and takes appropriate actions.
 * @param i Index of the client in the pollfds vector.
 */
void	Server::handleClient(size_t i)
{
	char	*buffer;
	buffer = (char *)calloc(sizeof(char), 1024);
	ssize_t n = recv(_pollfds[i].fd, buffer, 1024, 0);
	if ((n == -1) && (errno != EAGAIN && errno != EWOULDBLOCK))
	{
		perror("Data reception error.");
		close(_pollfds[i].fd);
		_pollfds.erase(_pollfds.begin() + i);
		_clients.erase(_clients.begin() + i - 1);
	}
	else if (n == 0)
	{
		std::cout << "<- " << intToIp(this->_clients[i - 1]->getAddr()) << " : Disconnected." << std::endl;

		close(_pollfds[i].fd);
		_pollfds.erase(_pollfds.begin() + i);
		Client *client_temp = this->_clients[i - 1];
		_clients.erase(_clients.begin() + i - 1);
		delete (client_temp);
	}
	else
	{
		std::string buffer2(buffer);
		if (buffer2.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
			this->_clients[i - 1]->addBuffer(buffer2);
		else
		{
			free (buffer);
			return ;
		}

		if (buffer2.find("\n") != std::string::npos)
		{
			bool shouldRemoveClient = this->treatBuffer(*this->_clients[i - 1]);
			if (shouldRemoveClient)
			{
				close(_pollfds[i].fd);
				_pollfds.erase(_pollfds.begin() + i);
				_clients.erase(_clients.begin() + i - 1);
			}
		}
	}
	free(buffer);
}

/**
 * Processes the buffer of a client, interprets the commands, and takes appropriate actions.
 * @param client Reference to the client whose buffer is to be treated.
 * @return true if the client should be removed, false otherwise.
 */
bool	Server::treatBuffer(Client &client)
{

	std::string buffer = client.getBuffer();
	std::map<std::string, function_t>::iterator it;
	size_t	pos;

	while ((pos = buffer.find("\n")) != std::string::npos)
	{
		std::string line = buffer.substr(0, pos);
		if (!client.getAuth())
		{
			int ret = client.checkAuth(*this, _password, line);
			if (ret == 2)
			{
				send(client.getFd(), "464 : Password is incorrect.\r\n", 35, 0);
				std::cout << "<- " << intToIp(client.getAddr()) << " : Connexion refused (wrong password)." << std::endl;
				return (true);
			}
		}
		else
		{
			std::vector <std::string> cmd_param = splitString(line, ' ');
			cmd_param[0] = toUpperString(cmd_param[0]);
			switchCmd(cmd_param, client, *this);
		}
		buffer.erase(0, pos + 1);
	}
	client.clearBuffer();
	return (false);
}

/**
 * Retrieves a client by its nickname.
 * @param nickname The nickname of the client to be retrieved.
 * @return Pointer to the client with the given nickname, or NULL if not found.
 */
Client*	Server::getClientByNickname(const std::string& nickname)
{
	const std::vector<Client*>& clients = getClients();
	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			return *it;
		}
	}
	return (NULL);
}

/**
 * Finds the index of a channel by its name.
 * @param channelName The name of the channel to be found.
 * @return Index of the channel if found, -1 otherwise.
 */
int	Server::findChannel(const std::string &channelName)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].getName() == channelName)
			return static_cast<int>(i);
	}
	return -1;
}

/**
 * Finds the index of a client by its nickname.
 * @param nickname The nickname of the client to be found.
 * @return Index of the client if found, -1 otherwise.
 */
int	Server::findClient(const std::string &nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getNickname() == nickname)
			return static_cast<int>(i);
	}
	return -1;
}

/**
 * Adds a new client to the server's client list.
 * @param client Pointer to the client to be added.
 */
void	Server::addClient(Client *client)
{
	_clients.push_back(client);
}

/**
 * Shuts down the server by setting its status to off.
 */
void Server::setOff()
{
	this->_isOn = false;
}
