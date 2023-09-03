#include "../commands.hpp"

/**
 * Checks if the provided password for the bot command is correct.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the command.
 * @param password  The password string to be checked.
 * @return          Returns 1 if the password is correct, 0 otherwise.
 */
static int	checkPassBot(Server &server, Client &client, std::string password)
{
	if (password != "42Nice")
	{
		std::string error = ":" + server.getName() + " 461 " + client.getNickname() + " BOT :Wrong password\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to use BOT command with wrong password." << std::endl;
		return (0);
	}
	return (1);
}

static int	checkBotExists(Server &server, Client &client, std::vector<std::string> param)
{
	if (server.channels[server.findChannel(param[3])].getBot() != NULL)
	{
		if (param[2] == "CREATE")
		{
			std::string error = ":" + server.getName() + " 461 " + client.getNickname() + " BOT : already exists\r\n";
			send(client.getFd(), error.c_str(), error.length(), 0);
			std::cout << "** BOT ERROR : " << intToIp(client.getAddr()) << " tried to create BOT but already exists." << std::endl;
		}
		return (1);
	}
	return (0);
}

/**
 * Checks if a specified channel exists.
 * @param server       Reference to the server object where the action will take place.
 * @param client       Reference to the client object initiating the command.
 * @param channelName  The name of the channel to be checked.
 * @return             Returns 1 if the channel exists, 0 otherwise.
 */
static int	checkChannelExist(Server &server, Client &client, std::string channelName)
{
	int chan_index = server.findChannel(channelName);
	if (chan_index == -1)
	{
		std::string error = ":" + server.getName() + " 461 " + client.getNickname() + " BOT :Channel doesn't exist\r\n";
		send(client.getFd(), error.c_str(), error.length(), 0);
		std::cout << "** ERROR : " << intToIp(client.getAddr()) << " tried to create BOT in a channel that doesn't exist." << std::endl;
		return (0);
	}
	return (1);
}

/**
 * Creates a new bot and adds it to the specified channel.
 * @param server       Reference to the server object where the action will take place.
 * @param botName      The name of the bot to be created.
 * @param channelName  The name of the channel where the bot will be added.
 */
static void	createBot(Server &server, std::string botName, std::string channelName)
{
	Client *newBot = new Client();
	newBot->setNickname("BOT");
	newBot->setUsername("BOT");
	newBot->setRealname("BOT");
	newBot->setAddr(16777343);
	server.channels[server.findChannel(channelName)].setBot(newBot);
	server.channels[server.findChannel(channelName)].addClients(botName);
	std::cout << "** BOT INFO : " << botName << " created." << std::endl;
}

/**
 * Adds a bot to a specified channel.
 * @param server       Reference to the server object where the action will take place.
 * @param channelName  The name of the channel where the bot will be added.
 */
static void	addBotToChannel(Server &server, std::string channelName)
{
	int chan_nbr = server.findChannel(channelName);
	Client &bot = *server.channels[chan_nbr].getBot();
	server.channels[chan_nbr].notifyUserJoin(bot, server);
}

/**
 * Makes the bot send a message to a specified channel.
 * @param server       Reference to the server object where the action will take place.
 * @param channelName  The name of the channel where the message will be sent.
 * @param messageText  The content of the message to be sent.
 */
static void talkBot(Server &server, std::string channelName, std::string messageText)
{
	std::string botName = server.channels[server.findChannel(channelName)].getBot()->getNickname();
	std::string message = ":" + botName + "!~" + botName + "@127.0.0.1 PRIVMSG " + channelName + " :" + messageText + "\r\n";
	server.channels[server.findChannel(channelName)].sendToChannelMembers(server, message, "None");
	std::cout << "** BOT INFO : " << botName << " sent a message to channel " << channelName << "." << std::endl;
}

/**
 * Initializes a bot in a specified channel.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the command.
 * @param param     Vector containing the command details.
 * @return          Returns 0 after execution.
 */
static int	initBot(Server &server, Client &client, std::vector<std::string> param)
{
	if (!checkPassBot(server, client, param[1]))
		return (0);
	if (checkChannelExist(server, client, param[3]) && !checkBotExists(server, client, param))
	{
		createBot(server, "BOT", param[3]);
		addBotToChannel(server, param[3]);
		talkBot(server, param[3], "Hello, I'm BOT and I'm here to serve you.");
	}
	return (0);
}

/**
 * Removes a bot from a specified channel.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the command.
 * @param param     Vector containing the command details.
 * @return          Returns 0 after execution.
 */
static int	removeBot(Server &server, Client &client, std::vector<std::string> param)
{
	if (!checkPassBot(server, client, param[1]))
		return (0);
	if (checkBotExists(server, client, param) && checkChannelExist(server, client, param[3]))
	{
		std::string QuitMessage = ":BOT!~BOT@127.0.0.1 PART " + param[3] + " :My job is done here.\r\n";
		server.channels[server.findChannel(param[3])].sendToChannelMembers(server, QuitMessage, "NONE");
		delete (server.channels[server.findChannel(param[3])].getBot());
		server.channels[server.findChannel(param[3])].setBot(NULL);
		server.channels[server.findChannel(param[3])].removeMember(param[3]);
		std::cout << "** BOT INFO : " << param[3] << " bot removed." << std::endl;
	}
	return (0);
}

/**
 * Makes the bot kick a user from a specified channel.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object that will be kicked.
 * @param chan_index Index of the channel from which the user will be kicked.
 * @param reason    Reason for the kick.
 */
void	kickByBot(Server &server, Client &client, int chan_index, std::string reason)
{
	Client &bot = *server.channels[chan_index].getBot();
	std::string	target = client.getNickname();
	std::string	opt_message = "";
	std::string botName = server.channels[chan_index].getBot()->getNickname();
	std::string channelName = server.channels[chan_index].getName();

	std::string message = ":" + botName + "!~" + botName + "@127.0.0.1 KICK " + channelName + " " + target + " :" + reason + "\r\n";
	server.channels[chan_index].sendToChannelMembers(server, message, "none");
	server.channels[chan_index].kick(target);
	std::cout << "** BOT INFO : " << bot.getNickname() << " kicked " << target << " from channel " << server.channels[chan_index].getName() << "(" << reason << ")." << std::endl;
}

/**
 * Checks if a message contains forbidden words.
 * @param words     Vector containing the words of the message.
 * @return          Returns 1 if a forbidden word is found, 0 otherwise.
 */
int checkGrossBot(const std::vector<std::string>& words)
{
	std::vector<std::string> forbiddenWords;
	forbiddenWords.push_back("fuck");
	forbiddenWords.push_back("macron");
	forbiddenWords.push_back("shit");

	for (std::vector<std::string>::const_iterator it = words.begin(); it != words.end(); ++it)
	{
		std::string lowercaseWord = *it;
		std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), static_cast<int(*)(int)>(std::tolower));

		if (std::find(forbiddenWords.begin(), forbiddenWords.end(), lowercaseWord) != forbiddenWords.end())
			return 1;
	}
	return 0;
}

/**
 * Processes and executes bot-related commands.
 * @param server    Reference to the server object where the action will take place.
 * @param client    Reference to the client object initiating the command.
 * @param param     Vector containing the command details.
 * @return          Returns 0 after execution.
 */
int		BOT(Server &server, Client &client, std::vector<std::string> param)
{
	if (param.size() == 4 && param[2] == "CREATE")
		initBot(server, client, param);
	else if (param.size() == 4 && param[2] == "REMOVE")
		removeBot(server, client, param);
	else if (param.size() > 4 && param[2] == "SAY")
	{
		if (!checkPassBot(server, client, param[1]))
			return (0);
		if (checkBotExists(server, client, param) && checkChannelExist(server, client, param[3]))
		{
			std::string messageText;
			for (size_t i = 4; i < param.size(); i++)
			{
				messageText += param[i];
				if (i < param.size() - 1)
					messageText += " ";
			}
			talkBot(server, param[3], messageText);
		}
	}
	return (0);
}
