#include "../commands.hpp"

/**
 * Processes the QUIT command from a client.
 * The QUIT command is used by clients to indicate that they are disconnecting from the server.
 * This function handles the client's disconnection, broadcasts the quit message to all channels
 * the client is a member of, and logs the disconnection event.
 * @param server     Reference to the server object.
 * @param client     Reference to the client object sending the QUIT command.
 * @param arguments  Vector containing the command details, including the optional quit message.
 * @return           Returns 0 after processing the QUIT command.
 */
int	QUIT(Server &server, Client &client, std::vector<std::string> arguments)
{
	std::string quitMessage;

	if (arguments.size() < 2)
		quitMessage = "Client Quit";
	else
	{
		if (arguments[1][0] == ':')
			quitMessage = &arguments[1][1];
		else
			quitMessage = arguments[1];
		for (size_t i = 2; i < arguments.size(); ++i)
		{
			quitMessage += " " + arguments[i];
		}
	}

	std::string formattedQuitMessage = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + intToIp(client.getAddr()) + " QUIT :" + quitMessage + "\r\n";

	for (size_t i = 0; i < server.channels.size(); ++i)
	{
		if (server.channels[i].isMember(client.getNickname()))
			server.channels[i].sendToChannelMembers(server, formattedQuitMessage, client.getNickname());
	}

	std::cout << "** INFO : " << intToIp(client.getAddr()) << " has quit the server. Quit message: " << quitMessage << std::endl;
	return (0);
}
