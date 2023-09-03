#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include "Client.hpp"
#include "Server.hpp"

class Server;
class Client;

class Channel
{
	public:
		Channel(std::string input);
		~Channel(void);
		void		promote(std::string target);
		std::string	getName(void) const;
		std::string	getTopic(void) const;
		void		setTopic(std::string topic);
		Client*		getBot(void) const;
		void		setBot(Client* bot);
		const std::vector<std::string>&	getClients() const;
		const std::vector<std::string>&	getOperators() const;
		void		addClients(std::string name);
		void		addOperator(std::string name);
		void		removeOperator(std::string name);
		bool		isIn(std::string target);
		bool		isOperator(std::string target);
		void		ban(std::string target);
		void		kick(std::string target);

		bool		isBlacklisted(std::string target);
		void		addBlacklist(std::string target);
		void		removeBlacklist(std::string target);

		void		notifyUserJoin(Client& newUser, Server& server);
		void		sendToChannelMembers(Server &server, const std::string &message, const std::string &excludeNickname);
		std::string	getConnectedClientsAsString() const;
		bool		isMember(const std::string &nickname) const;
		void		removeMember(const std::string &nickname);

		bool		checkFlag(char target) const;
		void		addFlag(char input);
		std::string	getFlags(void) const;
		void		removeFlag(char input);

		bool		checkInvited(std::string target) const;
		void		addInvited(std::string input);
		void		removeInvited(std::string input);

		void		addVoiced(std::string input);
		void		removeVoiced(std::string input);
		bool		isVoiced(std::string target) const;
		const std::vector<std::string>&	getVoiced(void) const;

		std::string	getPassword(void) const;
		void		setPassword(std::string password);

		void		setMaxUsers(int maxUsers);
		int			getMaxUsers(void) const;

	private:
		Channel(void);
		std::string					_name;
		std::string					_topic;
		std::string					_password;
		int							_maxUsers;
		std::vector <std::string>	_clients;
		std::vector <std::string>	_operators;
		std::vector <std::string>	_blacklist;
		std::vector <std::string>	_invited;
		std::vector <std::string>	_voiced;
		std::string					_flags;
		Client*						_bot;
};

#endif
