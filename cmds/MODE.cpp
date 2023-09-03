#include "../commands.hpp"

/**
 * Promotes a user to operator status in a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the promotion.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the promotion is successful, 0 otherwise.
 */
int	MODEOP(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			// First check if the client is an operator
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					// Then check if the target is in the channel
					for (it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++)
					{
						if (*it2 == name[3])
						{
							it->promote(name[3]);
							std::string response = ":" + server.getName() + " 324 " + name[3] + " " + name[1] + " +o\r\n";
							it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
							std::cout << "** INFO : " << intToIp(client.getAddr()) << " promoted " << name[3] << " to operator" << std::endl;
							return (1);
						}
					}
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to promote " << name[3] << " to operator : user not in channel: " << name[1] << std::endl;
					std::string response = ":" + server.getName() + " 442 " + name[3] + " " + name[1] + " :The user isn't part of this channel\r\n";
					send(client.getFd(), response.c_str(), response.size(), 0);
					return (0);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to promote " << name[3] << " to operator : user not an operator: " << name[1] << std::endl;
			std::string response = ":" + server.getName() + " 482 " + name[3] + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to promote " << name[3] << " to operator : channel not found: "<< name[1] << std::endl;
	std::string response = ":" + server.getName() + " 403 " + name[3] + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Demotes a user from operator status in a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the demotion.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the demotion is successful, 0 otherwise.
 */
int	MODEDEOP(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			// First check if the client is an operator
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					// Then check if the target is in the channel
					for (it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++)
					{
						if (*it2 == name[3])
						{
							it->removeOperator(name[3]);
							std::string response = ":" + server.getName() + " 324 " + name[3] + " " + name[1] + " -o\r\n";
							it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
							std::cout << "** INFO : " << intToIp(client.getAddr()) << " demoted " << name[3] << " from operator" << std::endl;
							return (1);
						}
					}
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to demote " << name[3] << " from operator : user not in channel: " << name[1] << std::endl;
					std::string response = ":" + server.getName() + " 442 " + name[3] + " " + name[1] + " :The user isn't part of this channel\r\n";
					send(client.getFd(), response.c_str(), response.size(), 0);
					return (0);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to demote " << name[3] << " from operator : user not an operator: " << name[1] << std::endl;
			std::string response = ":" + server.getName() + " 482 " + name[3] + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to demote " << name[3] << " from operator : channel not found: "<< name[1] << std::endl;
	std::string response = ":" + server.getName() + " 403 " + name[3] + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Promotes a user to voiced status in a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the promotion.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the promotion is successful, 0 otherwise.
 */
int	MODEVOICE(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			// First check if the client is an operator
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					// Then check if the target is in the channel
					for (it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++)
					{
						if (*it2 == name[3])
						{
							it->addVoiced(name[3]);
							std::string response = ":" + server.getName() + " 324 " + name[3] + " " + name[1] + " +v\r\n";
							it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
							std::cout << "** INFO : " << intToIp(client.getAddr()) << " promoted " << name[3] << " to voiced" << std::endl;
							return (1);
						}
					}
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to promote " << name[3] << " to voiced : user not in channel: " << name[1] << std::endl;
					std::string response = ":" + server.getName() + " 442 " + name[3] + " " + name[1] + " :The user isn't part of this channel\r\n";
					send(client.getFd(), response.c_str(), response.size(), 0);
					return (0);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to promote " << name[3] << " to voiced : user not an operator: " << name[1] << std::endl;
			std::string response = ":" + server.getName() + " 482 " + name[3] + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to promote " << name[3] << " to voiced : channel not found: "<< name[1] << std::endl;
	std::string response = ":" + server.getName() + " 403 " + name[3] + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Demotes a user from voiced status in a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the demotion.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the demotion is successful, 0 otherwise.
 */
int	MODEDEVOICE(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			// First check if the client is an operator
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					// Then check if the target is in the channel
					for (it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++)
					{
						if (*it2 == name[3])
						{
							it->removeVoiced(name[3]);
							std::string response = ":" + server.getName() + " 324 " + name[3] + " " + name[1] + " -v\r\n";
							it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
							std::cout << "** INFO : " << intToIp(client.getAddr()) << " demoted " << name[3] << " from voiced" << std::endl;
							return (1);
						}
					}
					std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to demote " << name[3] << " from voiced : user not in channel: " << name[1] << std::endl;
					std::string response = ":" + server.getName() + " 442 " + name[3] + " " + name[1] + " :The user isn't part of this channel\r\n";
					send(client.getFd(), response.c_str(), response.size(), 0);
					return (0);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to demote " << name[3] << " from voiced : user not an operator: " << name[1] << std::endl;
			std::string response = ":" + server.getName() + " 482 " + name[3] + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to demote " << name[3] << " from voiced : channel not found: " << name[1] << std::endl;
	std::string response = ":" + server.getName() + " 403 " + name[3] + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the invite-only mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object initiating the mode change.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode change is successful, 0 otherwise.
 */
int	INVITE_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			// First check if the client is an operator
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('i');
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +i\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set invite mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the invite mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the invite mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the moderate mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully set, 0 otherwise.
 */
int	MODERATE_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('m');
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +m\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set moderate mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the moderate mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the moderate mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the private mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully set, 0 otherwise.
 */
int	PRIVATE_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('p');
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +t\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set topic mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the topic mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the topic mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the topic mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully set, 0 otherwise.
 */
int	TOPIC_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('t');
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +t\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set topic mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the topic mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the topic mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the password mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully set, 0 otherwise.
 */
int	SET_PASSWORD_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('k');
					it->setPassword(name[3]);
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +k\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set password mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Unsets the password mode for a specified channel.
 *
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to unset the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully unset, 0 otherwise.
 */
int	UNSET_PASSWORD_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->removeFlag('k');
					it->setPassword("");
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " -k\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " unset password mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to unset the password mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to unset the password mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the user limit mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully set, 0 otherwise.
 */
int	SET_LIMIT_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the password mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('l');
					it->setMaxUsers(atoi(name[3].c_str()));
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +l " + name[3] + "\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set limit mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the limit mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the limit mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Unsets the user limit mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to unset the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully unset, 0 otherwise.
 */
int	UNSET_LIMIT_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->removeFlag('l');
					it->setMaxUsers(0);
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " -l\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " unset limit mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to unset the limit mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to unset the limit mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Sets the ban mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to set the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully set, 0 otherwise.
 */
int	BAN_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4 && name.size() != 3)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the ban mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->addFlag('b');
					if (name.size() == 4)
					{
						it->addBlacklist(name[3]);
						std::vector<std::string> formattedStr;
						formattedStr.push_back("KICK");
						formattedStr.push_back(name[1]);
						formattedStr.push_back(name[3]);
						formattedStr.push_back("You have been banned from this channel");
						KICK(server, client, formattedStr);
					}
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +b\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set ban mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the ban mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the ban mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Unsets the ban mode for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object attempting to unset the mode.
 * @param name    Vector containing the command details.
 * @return        Returns 1 if the mode is successfully unset, 0 otherwise.
 */
int	UNBAN_MODE(Server &server, Client &client, std::vector<std::string> name)
{
	if (name.size() != 4 && name.size() != 3)
	{
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the unban mode of channel: " << name[1] << " invalid number of arguments" << std::endl;
		std::string response = ":" + server.getName() + " 461 " + client.getNickname() + " " + name[0] + " :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return (0);
	}
	std::vector<Channel>::iterator it;
	for (it = server.channels.begin(); it != server.channels.end(); it++)
	{
		if (it->getName() == name[1])
		{
			std::vector<std::string>::const_iterator it2;
			for (it2 = it->getOperators().begin(); it2 != it->getOperators().end(); it2++)
			{
				if (*it2 == client.getNickname())
				{
					it->removeFlag('b');
					if (name.size() == 4)
					{
						it->removeBlacklist(name[3]);
					}
					std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " -b\r\n";
					it->sendToChannelMembers(server, response, "aaaaaaaaaaa");
					std::cout << "** INFO : " << intToIp(client.getAddr()) << " set unban mode on " << name[1] << std::endl;
					return (1);
				}
			}
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the unban mode of channel: " << name[1] << " user is not operator" << std::endl;
			std::string response = ":" + server.getName() + " 482 " + client.getNickname() + " " + name[1] + " :You're not a channel operator\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to set the unban mode of channel: " << name[1] << " channel not found" << std::endl;
	std::string response = ":" + server.getName() + " 403 " + client.getNickname() + " " + name[1] + " :No such channel\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	return (0);
}

/**
 * Initializes a map of mode functions.
 * @return        Returns a map linking mode strings to their corresponding functions.
 */
typedef int (*ModeFunction)(Server&, Client&, std::vector<std::string>);
std::map<std::string, ModeFunction> initializeModeMap()
{
	std::map<std::string, ModeFunction> modeMap;
	modeMap.insert(std::make_pair("+o", MODEOP));
	modeMap.insert(std::make_pair("-o", MODEDEOP));
	modeMap.insert(std::make_pair("o", MODEOP));
	modeMap.insert(std::make_pair("+v", MODEVOICE));
	modeMap.insert(std::make_pair("-v", MODEDEVOICE));
	modeMap.insert(std::make_pair("v", MODEVOICE));
	modeMap.insert(std::make_pair("+i", INVITE_MODE));
	modeMap.insert(std::make_pair("i", INVITE_MODE));
	modeMap.insert(std::make_pair("+m", MODERATE_MODE));
	modeMap.insert(std::make_pair("m", MODERATE_MODE));
	modeMap.insert(std::make_pair("+p", PRIVATE_MODE));
	modeMap.insert(std::make_pair("p", PRIVATE_MODE));
	modeMap.insert(std::make_pair("+t", TOPIC_MODE));
	modeMap.insert(std::make_pair("t", TOPIC_MODE));
	modeMap.insert(std::make_pair("+k", SET_PASSWORD_MODE));
	modeMap.insert(std::make_pair("-k", UNSET_PASSWORD_MODE));
	modeMap.insert(std::make_pair("k", SET_PASSWORD_MODE));
	modeMap.insert(std::make_pair("+l", SET_LIMIT_MODE));
	modeMap.insert(std::make_pair("-l", UNSET_LIMIT_MODE));
	modeMap.insert(std::make_pair("l", SET_LIMIT_MODE));
	modeMap.insert(std::make_pair("+b", BAN_MODE));
	modeMap.insert(std::make_pair("-b", UNBAN_MODE));
	modeMap.insert(std::make_pair("b", BAN_MODE));
	return (modeMap);
}

/**
 * Handles the MODE command for a specified channel.
 * @param server  Reference to the server object.
 * @param client  Reference to the client object sending the MODE command.
 * @param name    Vector containing the command details.
 * @return        Returns 0 after processing the command.
 */
int	MODE(Server &server, Client &client, std::vector<std::string> name)
{
	std::map<std::string, ModeFunction> modeFunctions = initializeModeMap();

	if (name.size() < 2)
	{
		std::cout << "** INFO : " << intToIp(client.getAddr()) << " failed to send MODE command : not enough arguments" << std::endl;
		return (1);
	}
	else if (name.size() == 2)
	{
		std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +ovimptklb\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::string mode = name[2];

		std::map<std::string, ModeFunction>::iterator it = modeFunctions.find(mode);
		if (it != modeFunctions.end())
			it->second(server, client, name);
		else
		{
			std::cout << "** ERROR : " << intToIp(client.getAddr()) << " failed to send MODE command : unknown mode" << std::endl;
			std::string response = ":" + server.getName() + " 472 " + client.getNickname() + " " + mode + " :is unknown mode char to me\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return (0);
		}
	}
	return (0);
}
