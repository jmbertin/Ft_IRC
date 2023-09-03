#pragma once

#include <iostream>
#include "Server.hpp"
#include <sys/time.h>

class Server;

class Client
{
	public:
		Client();
		~Client();
		Client(int fd, struct sockaddr_in client_addr);
		int				getFd() const;
		void			addBuffer(std::string str);
		void			clearBuffer();
		int				checkAuth(Server& server, std::string password, std::string line);
		std::string		getBuffer() const;
		std::string		getNickname() const;
		void			setNickname(std::string nick);
		std::string		getUsername() const;
		void			setUsername(std::string username);
		std::string		getRealname() const;
		void			setRealname(std::string realname);
		bool			getAuth() const;
		in_addr_t		getAddr() const;
		void			setNicked(bool nicked);
		int				getChanNbr(void);
		void			setChanNbr(int new_chan_nbr);
		int 			getMessageCount();
		void 			setMessageCount(int messageCount);
		struct timeval	getLastMessageTime();
		void			setLastMessageTime(struct timeval lastMessageTime);
		void			setAddr(int ip);

	private:
		Client(Client const &src);
		Client &operator=(Client const &rhs);
		int					_fd;
		bool				_isAuthentified;
		bool				_isNicked;
		bool				_isUsered;
		bool				_isPassworded;
		std::string			_nickname;
		std::string			_username;
		std::string			_realname;
		struct sockaddr_in	_clientAddr;
		std::string			_buffer;
		time_t				_timeConnection;
		int					_chanNbr;
		int 				_messageCount;
		struct timeval		_lastMessageTime;
};
