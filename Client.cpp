#include "Client.hpp"
#include "commands.hpp"

/**
 * Default constructor for the Client class.
 * Initializes the client with default values.
 */
Client::Client(): _fd(0), _isAuthentified(false), _isNicked(false), _isUsered(false), _isPassworded(false), _messageCount(0)
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	_lastMessageTime = currentTime;
}

/**
 * Parameterized constructor for the Client class.
 * Initializes the client with a file descriptor and client address.
 * @param fd The file descriptor associated with the client.
 * @param client_addr The address of the client.
 */
Client::Client(int fd, struct sockaddr_in client_addr): _fd(fd), _isAuthentified(false), _isNicked(false), _isUsered(false), _isPassworded(false), _clientAddr(client_addr), _chanNbr(0), _messageCount(0)
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	_lastMessageTime = currentTime;
}

/**
 * Destructor for the Client class.
 */
Client::~Client() { }

/**
 * Copy constructor for the Client class.
 * @param src The client object to be copied.
 */
Client::Client(Client const &src): _fd(src._fd), _isAuthentified(src._isAuthentified), _isNicked(src._isNicked), _isUsered(src._isUsered), _isPassworded(src._isPassworded), _messageCount(0)
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	_lastMessageTime = currentTime;
}

/**
 * Overloaded assignment operator for the Client class.
 * @param rhs The client object to be assigned.
 * @return A reference to the current client object.
 */
Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		this->_fd = rhs._fd;
		this->_isAuthentified = rhs._isAuthentified;
		this->_isNicked = rhs._isNicked;
		this->_isUsered = rhs._isUsered;
		this->_isPassworded = rhs._isPassworded;
	}
	(void) this->_clientAddr;
	(void) this->_timeConnection;
	return (*this);
}

/**
 * Retrieves the file descriptor associated with the client.
 * @return The file descriptor of the client.
 */
int			Client::getFd() const { return (this->_fd); }

/**
 * Appends a string to the client's buffer.
 * @param str The string to be appended.
 */
void		Client::addBuffer(std::string str) { this->_buffer += str; }

/**
 * Clears the client's buffer.
 */
void		Client::clearBuffer() { this->_buffer = ""; }

/**
 * Retrieves the client's buffer.
 * @return The client's buffer.
 */
std::string	Client::getBuffer() const { return (this->_buffer); }

/**
 * Retrieves the client's nickname.
 * @return The client's nickname.
 */
std::string	Client::getNickname() const { return (this->_nickname); }

/**
 * Sets the client's nickname.
 * @param nick The nickname to be set.
 */
void		Client::setNickname(std::string nick) { this->_nickname = nick; }

/**
 * Retrieves the client's username.
 * @return The client's username.
 */
std::string	Client::getUsername() const { return (this->_username); }

/**
 * Sets the client's username.
 * @param username The username to be set.
 */
void		Client::setUsername(std::string username) { this->_username = username; }

/**
 * Retrieves the client's real name.
 * @return The real name of the client.
 */
std::string	Client::getRealname() const { return (this->_realname); }

/**
 * Sets the client's real name.
 * @param realname The new real name for the client.
 */
void		Client::setRealname(std::string realname) { this->_realname = realname; }

/**
 * Retrieves the number of channels the client is currently in.
 *
 * @return The number of channels.
 */
int			Client::getChanNbr(void) { return (this->_chanNbr); }

/**
 * Sets the number of channels the client is currently in.
 * @param new_chan_nbr The new number of channels.
 */
void		Client::setChanNbr(int new_chan_nbr) { this->_chanNbr = new_chan_nbr; }

/**
 * Checks if the client is authenticated.
 * @return True if authenticated, false otherwise.
 */
bool		Client::getAuth() const { return (this->_isAuthentified); }

/**
 * Retrieves the IP address of the client.
 * @return The IP address of the client.
 */
in_addr_t	Client::getAddr() const { return this->_clientAddr.sin_addr.s_addr; }

/**
 * Sets the IP address for the client.
 * @param ip The new IP address.
 */
void		Client::setAddr(int ip) { this->_clientAddr.sin_addr.s_addr = ip; }

/**
 * Sets the client's nicked status.
 * @param nicked The new nicked status.
 */
void		Client::setNicked(bool nicked) { this->_isNicked = nicked; }

/**
 * Checks the client's authentication status and processes various commands.
 * @param server The server instance.
 * @param password The password for authentication.
 * @param line The command line received from the client.
 * @return 1 if the client is authenticated, 2 if the password is incorrect, and 0 otherwise.
 */
int			Client::checkAuth(Server& server, std::string password, std::string line)
{
	if (line.find("CAP") != std::string::npos)
		CAP(server, *this, splitString(line, ' '));
	if (line.find("NICK") != std::string::npos)
	{
		if (NICK(server, *this, splitString(line, ' ')) == 1)
			this->_isNicked = true;
	}
	if (this->_isAuthentified)
		return (1);
	if (!_isPassworded)
	{
		if (line == ("PASS " + password + "\r") || line == ("PASS " + password))
			this->_isPassworded = true;
		else if (line.find("PASS") != std::string::npos)
			return (2);
	}
	if (!_isUsered && line.find("USER") != std::string::npos)
	{
		if (USER(server, *this, splitString(line, ' ')) == 1)
			this->_isUsered = true;
	}
	if (_isNicked && _isUsered && _isPassworded)
	{
		std::string host = this->_realname;
		std::string welcomeMessage = ":" + server.getName() + " 001 " + this->_nickname + " :Welcome to the IRC Network, " + this->_nickname + "!" + this->_username + "@" + intToIp(this->getAddr()) + "\r\n";
		send(this->_fd, welcomeMessage.c_str(), welcomeMessage.size(), 0);

		std::string serverMessage = ":" + server.getName() + " 002 " + this->_nickname + " :Your host is " + server.getName() + ", running custom c++ homemade version\r\n";
		send(this->_fd, serverMessage.c_str(), serverMessage.size(), 0);

		std::string createdMessage = ":" + server.getName() + " 003 " + this->_nickname + " :This server was created (Hum... I don't remember actually)\r\n";
		send(this->_fd, createdMessage.c_str(), createdMessage.size(), 0);

		std::string serverInfoMessage = ":" + server.getName() + " 004 " + this->_nickname + " " + server.getName() + " MACOSX (oiwszcerkfydnx) (bciklmnoprstv)\r\n";
		send(this->_fd, serverInfoMessage.c_str(), serverInfoMessage.size(), 0);

		this->_isAuthentified = true;

		std::cout << "-> " << intToIp(this->getAddr()) << " : Successfuly identified." << std::endl;
		std::cout << "** INFOS : Uname = " << this->_username << ", Nname = " << this->_nickname;
		std::cout << ", Rname = " << this->_realname << "." << std::endl;
		return (1);
	}

	return (0);
}

/**
 * Retrieves the count of messages sent by the client.
 * @return The number of messages sent by the client.
 */
int			Client::getMessageCount() { return (this->_messageCount); }

/**
 * Sets the count of messages sent by the client.
 * @param messageCount The new message count.
 */
void		Client::setMessageCount(int messageCount) { this->_messageCount = messageCount; }

/**
 * Retrieves the timestamp of the last message sent by the client.
 * @return The timestamp of the last message.
 */
struct timeval	Client::getLastMessageTime() { return (this->_lastMessageTime); }

/**
 * Sets the timestamp of the last message sent by the client.
 * @param lastMessageTime The new timestamp for the last message.
 */
void		Client::setLastMessageTime(struct timeval lastMessageTime) { this->_lastMessageTime = lastMessageTime;}
