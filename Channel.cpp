#include "Channel.hpp"

/**
 * Constructs a new Channel object with a specified name.
 * @param input The name of the channel to be created.
 */
Channel::Channel(std::string input): _name(input), _topic(""), _flags("bt"), _bot(NULL) { }

/**
 * Destructor for the Channel class.
 */
Channel::~Channel(void) { }

/**
 * Promotes a user to operator status in the channel.
 * @param target The nickname of the user to promote.
 */
void	Channel::promote(std::string target)
{
	if (!(std::find(this->_operators.begin(), this->_operators.end(), target) !=
		this->_operators.end()))
	{
		for (size_t i = 0; i < this->_clients.size(); i++)
		{
			if (_clients[i] == target)
				this->_operators.push_back(target);
		}
	}
}

/**
 * Retrieves the name of the channel.
 * @return The name of the channel.
 */
std::string	Channel::getName(void) const { return(this->_name); }

/**
 * @return The topic of the channel.
 */
std::string	Channel::getTopic() const { return _topic; }

/**
 * Sets the topic of the channel.
 * @param topic The new topic to set.
 */
void	Channel::setTopic(std::string topic) { this->_topic = topic; }

/**
 * @return A list of clients currently in the channel.
 */
const std::vector<std::string>&	Channel::getClients() const { return _clients; }

/**
 * @return A list of operators in the channel.
 */
const std::vector<std::string>&	Channel::getOperators() const { return _operators; }

/**
 * Adds a client to the channel.
 * @param name The nickname of the client to add.
 */
void	Channel::addClients(std::string name)
{
	if (!(std::find(this->_clients.begin(), this->_clients.end(), name) !=
		this->_clients.end()))
	{
		this->_clients.push_back(name);
	}
}

/**
 * Adds a user as an operator in the channel.
 * @param name The nickname of the user to promote as an operator.
 */
void	Channel::addOperator(std::string name)
{
	if (!(std::find(this->_operators.begin(), this->_operators.end(), name) !=
		this->_operators.end()))
	{
		if (!(std::find(this->_clients.begin(), this->_clients.end(), name) !=
			this->_clients.end()))
				this->addClients(name);
		this->promote(name);
	}
}

/**
 * Removes a user from the operator list of the channel.
 * @param name The nickname of the user to demote.
 */
void	Channel::removeOperator(std::string name)
{
	if (std::find(this->_operators.begin(), this->_operators.end(), name) !=
		this->_operators.end())
	{
		for (size_t i = 0; i < this->_operators.size(); i++)
		{
			if (this->_operators[i] == name)
				this->_operators.erase(this->_operators.begin() + i);
		}
	}
}

/**
 * Adds a user to the voiced list of the channel.
 * @param name The nickname of the user to voice.
 */
void	Channel::addVoiced(std::string name)
{
	if (!(std::find(this->_voiced.begin(), this->_voiced.end(), name) !=
		this->_voiced.end()))
	{
		if (!(std::find(this->_clients.begin(), this->_clients.end(), name) !=
			this->_clients.end()))
				this->addClients(name);
		this->_voiced.push_back(name);
	}
}

/**
 * Removes a user from the voiced list of the channel.
 * @param name The nickname of the user to devoice.
 */
void	Channel::removeVoiced(std::string name)
{
	if (std::find(this->_voiced.begin(), this->_voiced.end(), name) !=
		this->_voiced.end())
	{
		for (size_t i = 0; i < this->_voiced.size(); i++)
		{
			if (this->_voiced[i] == name)
				this->_voiced.erase(this->_voiced.begin() + i);
		}
	}
}

/**
 * Checks if a user has voice privileges in the channel.
 * @param target The nickname of the user to check.
 * @return true if the user is voiced, false otherwise.
 */
bool	Channel::isVoiced(std::string target) const
{
	if (std::find(this->_voiced.begin(), this->_voiced.end(), target) !=
		this->_voiced.end())
		return (true);
	return (false);
}

/**
 * @return A list of voiced users in the channel.
 */
const std::vector<std::string>	&Channel::getVoiced(void) const { return _voiced; }

/**
 * Checks if a user is a member of the channel.
 * @param target The nickname of the user to check.
 * @return true if the user is a member, false otherwise.
 */
bool	Channel::isIn(std::string target)
{
	if (std::find(this->_clients.begin(), this->_clients.end(), target) !=
		this->_clients.end())
		return (true);
	return (false);
}

/**
 * Checks if a user is an operator in the channel.
 * @param target The nickname of the user to check.
 * @return true if the user is an operator, false otherwise.
 */
bool	Channel::isOperator(std::string target)
{
	if (std::find(this->_operators.begin(), this->_operators.end(), target) !=
		this->_operators.end())
		return (true);
	return (false);
}

/**
 * Checks if a user is blacklisted in the channel.
 * @param target The nickname of the user to check.
 * @return true if the user is blacklisted, false otherwise.
 */
bool	Channel::isBlacklisted(std::string target)
{
	if (std::find(this->_blacklist.begin(), this->_blacklist.end(), target) !=
		this->_blacklist.end())
		return (true);
	return (false);
}

/**
 * Adds a user to the blacklist of the channel.
 * @param target The nickname of the user to blacklist.
 */
void	Channel::addBlacklist(std::string target)
{
	if (!(std::find(this->_blacklist.begin(), this->_blacklist.end(), target) !=
		this->_blacklist.end()))
	{
		this->_blacklist.push_back(target);
	}
}

/**
 * Removes a user from the blacklist of the channel.
 * @param target The nickname of the user to remove from the blacklist.
 */
void	Channel::removeBlacklist(std::string target)
{
	if (std::find(this->_blacklist.begin(), this->_blacklist.end(), target) !=
		this->_blacklist.end())
	{
		for (size_t i = 0; i < this->_blacklist.size(); i++)
		{
			if (this->_blacklist[i] == target)
				this->_blacklist.erase(this->_blacklist.begin() + i);
		}
	}
}

/**
 * Notifies the channel members when a new user joins.
 * @param newUser The client object of the new user.
 * @param server The server object to fetch client details.
 */
void	Channel::notifyUserJoin(Client& newUser, Server& server)
{
	std::string joinMessage = ":" + newUser.getNickname() + "!" + newUser.getUsername() + "@" + intToIp(newUser.getAddr()) + " JOIN " + getName() + "\r\n";

	std::cout << "JOIN MESSAGE: " << joinMessage << std::endl;
	for (std::vector<std::string>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = server.getClientByNickname(*it);
		if (client)
			send(client->getFd(), joinMessage.c_str(), joinMessage.size(), 0);
	}
}

/**
 * Sends a message to all members of the channel.
 * @param server The server object to fetch client details.
 * @param message The message to send.
 * @param excludeNickname The nickname of the user to exclude from receiving the message.
 */
void	Channel::sendToChannelMembers(Server &server, const std::string &message, const std::string &excludeNickname)
{
	const std::vector<std::string>& _clients = this->getClients();
	for (std::vector<std::string>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != excludeNickname)
		{
			Client* client = server.getClientByNickname(*it);
			if (client)
			{
				send(client->getFd(), message.c_str(), message.length(), 0);
			}
		}
	}
}

/**
 * @return A string representation of all connected clients in the channel.
 */
std::string	Channel::getConnectedClientsAsString() const
{
	std::string clientsList;
	const std::vector<std::string> &clients = getClients();
	for (size_t i = 0; i < _clients.size(); i++)
	{
		clientsList += clients[i];
		if (i < _clients.size() - 1)
			clientsList += " ";
	}
	return clientsList;
}

/**
 * Checks if a given nickname is a member of the channel.
 * @param nickname The nickname to check.
 * @return true if the nickname is a member, false otherwise.
 */
bool	Channel::isMember(const std::string &nickname) const
{
	for (std::vector<std::string>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

/**
 * Removes a member from the channel.
 * @param nickname The nickname of the member to remove.
 */
void	Channel::removeMember(const std::string &nickname)
{
	for (std::vector<std::string>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == nickname)
		{
			_clients.erase(it);
			break;
		}
	}
}

/**
 * Checks if a given flag is set for the channel.
 * @param target The flag to check.
 * @return true if the flag is set, false otherwise.
 */
bool	Channel::checkFlag(char target) const
{
	if (std::find(this->_flags.begin(), this->_flags.end(), target) !=
		this->_flags.end())
		return (true);
	return (false);
}

/**
 * Adds a flag to the channel.
 * @param input The flag to add.
 */
void	Channel::addFlag(char input)
{
	if (this->checkFlag(input) == false)
		this->_flags = this->_flags + input;
}

/**
 * @return The flags set for the channel.
 */
std::string	Channel::getFlags(void) const { return (this->_flags); }

/**
 * Removes a flag from the channel.
 * @param input The flag to remove.
 */
void	Channel::removeFlag(char input)
{
	for (std::string::iterator it = _flags.begin(); it != _flags.end(); ++it)
	{
		if (*it == input)
		{
			_flags.erase(it);
			break;
		}
	}
}

/**
 * Sets a password for the channel.
 * @param input The password to set.
 */
void	Channel::setPassword(std::string input) { this->_password = input; }

/**
 * @return The password set for the channel.
 */
std::string	Channel::getPassword(void) const { return (this->_password); }

/**
 * Sets the maximum number of users allowed in the channel.
 * @param input The maximum number of users.
 */
void	Channel::setMaxUsers(int input) { this->_maxUsers = input; }

/**
 * @return The maximum number of users allowed in the channel.
 */
int	Channel::getMaxUsers(void) const { return (this->_maxUsers); }

/*
 * This function takes a nickname and returns if it is invited or not
 */
bool Channel::checkInvited(std::string target) const
{
	if (std::find(this->_invited.begin(), this->_invited.end(), target) != this->_invited.end())
		return (true);
	return (false);
}

/**
 * Adds a user to the invited list of the channel.
 * @param input The nickname of the user to invite.
 */
void	Channel::addInvited(std::string input)
{
	if (this->checkInvited(input) == false)
		this->_invited.push_back(input);
}

/**
 * Removes a user from the invited list of the channel.
 * @param input The nickname of the user to remove from the invited list.
 */
void	Channel::removeInvited(std::string input)
{
	for (std::vector <std::string>::iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		if (*it == input)
		{
			_invited.erase(it);
			break;
		}
	}
}

/**
 * Retrieves the bot associated with the channel.
 * @return A pointer to the bot client associated with the channel.
 */
Client*	Channel::getBot(void) const { return (this->_bot); }

/**
 * Sets a bot for the channel.
 * @param bot A pointer to the bot client to associate with the channel.
 */
void	Channel::setBot(Client* bot) { this->_bot = bot; }

/**
 * Kicks a user from the channel.
 * @param target The nickname of the user to kick.
 */
void	Channel::kick(std::string target)
{
	std::vector <std::string>::iterator itcl = find(this->_clients.begin(),
			this->_clients.end(), target);
	std::vector <std::string>::iterator itop = find(this->_operators.begin(),
			this->_operators.end(), target);
	if (itcl != this->_clients.end())
	{
		if (itop != this->_operators.end())
				this->_operators.erase(itop);
		this->_clients.erase(itcl);
	}
}
