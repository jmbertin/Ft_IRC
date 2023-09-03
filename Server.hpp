#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>
#include <map>
#include <stdio.h>

#include "commands.hpp"

#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Server;
class Channel;

typedef int (*function_t)(Server &, Client &, std::vector<std::string>);

class Server
{
	public:
		Server(std::string port, std::string password);
		~Server();

		int		getListenFd() const;
		void	mainLoop();
		void	acceptClientConnection();
		void	handleClient(size_t i);
		bool	treatBuffer(Client &client);
		Client*	getClientByNickname(const std::string& nickname);
		int		findChannel(const std::string &channelName);
		int		findClient(const std::string &nickname);
		void	addClient(Client *client);
		void	setOff(void);

		const std::vector<Client*>&	getClients(void) const;
		const std::string&			getName() const;
		std::vector<Channel>		channels;

	private:
		Server();
		Server(Server const &src);
		Server &operator=(Server const &rhs);

		std::string 						_password;
		std::string							_name;
		struct sockaddr_in					_serverAddr;
		std::vector<Client*>				_clients;
		std::vector<pollfd>					_pollfds;
		std::map<std::string, function_t>	_commands;
		int									_isOn;
};
