# Ft_IRC
Ft_IRC is a project undertaken as part of the 42 school curriculum. It's about creating our own IRC server according to standard RFC 2813 in C++ language.
This is a project that was particularly close to my heart, knowing that I myself was, in my youth, a regular user of IRC!
It's also a group project carried out by 3, with [Apyre83](https://github.com/Apyre83) and [Blablupo](https://github.com/blablupo).
We needed to choose a reference client, so we chose to use Colloquy, given that we were using OSX for development and testing.

**Developed and tested on MacOSX.**

----

## Features
**1. Channel Management:**
- Create and manage channels.
- Set and retrieve channel topics.
- Add and manage channel clients and operators.
- Handle voiced clients and manage voice permissions.
- Implement channel blacklists.
- Set channel flags and manage channel passwords.
- Limit the number of users in a channel.

**2. Client Management:**
- Handle client authentication and connection.
- Manage client buffer and process incoming messages.
- Set and retrieve client details like nickname, username, and real name.
- Track client message count and last message time.
- Handle client commands and interactions with the server.

**3. Command Handling:**
- A comprehensive set of IRC commands including BAN, BOT, CAP, DEOP, JOIN, KICK, MODE, NICK, NOTICE, PASS, PING, PRIVMSG, QUIT, TOPIC, and more.
- Dynamic command mapping for easy addition of new commands.
- Command error handling and feedback.

**4. Server Management:**
- Accept and manage client connections.
- Handle incoming client messages and distribute them appropriately.
- Search for clients by nickname and manage client lists.
- Graceful shutdown of the server.

**5. Robust Error Handling:**
- Handle various error scenarios like incorrect password, wrong port range, and data reception errors.
- Provide meaningful error messages to the user.

**6. Non-blocking Sockets:**
- Utilize non-blocking sockets for efficient communication.
- Implement a polling mechanism to handle multiple client connections simultaneously.

**7. Bot option:**
- Password protected
- Delegation of KICK, BAN, MUTE...
- Moderation of conversations (rudeness, insults, offense, etc.)
- Automatically greet new users of a channel
- Promotion and auto promotion

----

## Build and Installation

Clone the repository:

``git clone https://github.com/jmbertin/Ft_IRC/``

Navigate to the project directory and compile using Makefile:

``cd Ft_IRC``

``make``

The executable Ft_IRC will be created in the root directory. Use it as described in the Usage section.

----

## Usage

Usage

``././ircserv [port] [password]``

You have to choose server's port and a password server when launching the program.

To test the functionality, simply launch an IRC client of your choice. However, please note that some clients may send information slightly differently. As a result, it's not guaranteed that everything will be 100% operational outside of the reference client. It's recommended to use the reference client for the most consistent experience.

----

## Contribution
If you encounter any bugs or wish to add features, please feel free to open an issue or submit a pull request.
