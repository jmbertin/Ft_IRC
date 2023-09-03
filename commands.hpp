#pragma once

#include "defines.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>
#include <vector>
#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>
#include <cctype>
#include <errno.h>
#include <map>
#include <sstream>
#include <stdint.h>

class Server;
class Client;

int	BAN(Server &server, Client &client, std::vector<std::string> cmd);
int	BOT(Server &server, Client &client, std::vector<std::string> cmd);
int	CAP(Server &server, Client &client, std::vector<std::string> cmd);
int	DEOP(Server &server, Client &client, std::vector<std::string> cmd);
int	DEVOICE(Server &server, Client &client, std::vector<std::string> cmd);
int	EXITS(Server &server, Client &client, std::vector<std::string> param);
int	INVITE(Server &server, Client &client, std::vector<std::string> cmd);
int	ISON(Server &server, Client &client, std::vector<std::string> cmd);
int	JOIN(Server &server, Client &client, std::vector<std::string> cmd);
int	KICK(Server &server, Client &client, std::vector<std::string> cmd);
int	MODE(Server &server, Client &client, std::vector<std::string> cmd);
int	NICK(Server &server, Client &client, std::vector<std::string> cmd);
int	NOTICE(Server &server, Client &client, std::vector<std::string> cmd);
int	OP(Server &server, Client &client, std::vector<std::string> cmd);
int	PART(Server &server, Client &client, std::vector<std::string> cmd);
int	PASS(Server &server, Client &client, std::vector<std::string> cmd);
int	PING(Server &server, Client &client, std::vector<std::string> cmd);
int	PRIVMSG(Server &server, Client &client, std::vector<std::string> cmd);
int	QUIT(Server &server, Client &client, std::vector<std::string> cmd);
int	TOPIC(Server &server, Client &client, std::vector<std::string> cmd);
int	UNBAN(Server &server, Client &client, std::vector<std::string> cmd);
int	USER(Server &server, Client &client, std::vector<std::string> cmd);
int	VOICE(Server &server, Client &client, std::vector<std::string> cmd);
int	WHO(Server &server, Client &client, std::vector<std::string> cmd);

void	kickByBot(Server &server, Client &client, int chan_index, std::string reason);
int 	checkGrossBot(const std::vector<std::string>& words);

std::vector<std::string>	splitString(const std::string& input, char delimiter);
int							switchCmd(std::vector <std::string> param, Client &client, Server &server);
std::string					intToIp(uint32_t ip_address);
std::string					toUpperString(std::string input);
