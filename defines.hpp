#ifndef DEFINES_HPP
#define DEFINES_HPP

#define RPL_PONG 000
// Welcome
#define RPL_WELCOME 001

// Server information
#define RPL_LUSERCLIENT 251 // There are X users and Y servers.
#define RPL_LUSEROP 252 // Number of IRC operators.
#define RPL_LUSERCHANNELS 254 // Number of formed channels.
#define RPL_LUSERME 255 // You are connected to X servers and Y clients.

// Channel / User queries
#define RPL_MODECHANGE 324 // Indicates the channel mode.
#define RPL_AWAY 301
#define RPL_ISON 303
#define RPL_UNAWAY 305 // Indicates that the user is no longer marked as away.
#define RPL_NOWAWAY 306 // Indicates that the user is now marked as away.
#define RPL_NOTOPIC 331 // Indicates that no topic is set for the channel.
#define RPL_TOPIC 332 // Provides the channel topic.
#define RPL_BANLIST 367 // List of active bans for a channel.
#define RPL_ENDOFBANLIST 368 // Signals the end of the ban list.

// Errors
#define ERR_NOSUCHNICK 401 // Unknown or non-existent nickname.
#define ERR_NOSUCHCHANNEL 403 // Unknown or non-existent channel.
#define ERR_CANNOTSENDTOCHAN 404 // Cannot send a message to the channel (e.g., due to mode restriction or ban).
#define ERR_TOOMANYCHANNELS 405 // User has joined the maximum number of allowed channels.
#define ERR_NORECIPIENT 411 // No recipient given (for a command such as PRIVMSG or NOTICE).
#define ERR_NOTEXTTOSEND 412 // No text to send (for a command such as PRIVMSG or NOTICE).
#define ERR_UNKNOWNCOMMAND 421 // Unknown command or not recognized by the server.
#define ERR_NONICKNAMEGIVEN 431 // Auth: No nickname given when attempting to connect.
#define ERR_ERRONEUSNICKNAME 432 // Incorrect or invalid nickname.
#define ERR_NICKNAMEINUSE 433 // The chosen nickname is already in use by another user.
#define ERR_USERNOTINCHANNEL 441 // User is not present in the specified channel.
#define ERR_NOTONCHANNEL 442 // You are not in the specified channel.
#define ERR_USERONCHANNEL 443 // The specified user is already on the channel.
#define ERR_NOTREGISTERED 451 // You are not registered (you must register before using certain commands).
#define ERR_NEEDMOREPARAMS 461 // Missing parameters for a given command.
#define ERR_ALREADYREGISTRED 462 // You are already registered (may occur if you try to register multiple times).
#define ERR_PASSWDMISMATCH 464 // The password does not match (e.g., when connecting as an operator).
#define ERR_YOUREBANNEDCREEP 465 // You are banned from the server.
#define ERR_CHANISFULL 471 // The specified channel is full.
#define ERR_INVITEONLYCHAN 473 // The specified channel is for invited users only.
#define ERR_BANNEDFROMCHAN 474 // You are banned from the channel.
#define ERR_NOPRIVILEGES 481 // You do not have the necessary privileges to execute the command (usually reserved for IRC operators or administrators).
#define ERR_CHANOPRIVSNEEDED 482 // Channel operator privileges needed (for changing channel modes or managing users, for example).

//custom defines
#define ERR_INVALIDCHNAME 501

#endif
