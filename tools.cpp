#include "commands.hpp"

/**
 * Splits a string based on a given delimiter.
 * @param input The string to be split.
 * @param delimiter The character used to split the string.
 * @return A vector of substrings split by the delimiter.
 */
std::vector<std::string> splitString(const std::string& input, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;

	for (std::string::const_iterator it = input.begin(); it != input.end(); ++it)
	{
		if (*it == delimiter)
		{
			if (!token.empty())
			{
				if (token[token.size() - 1] == '\r')
					token.erase(token.size() - 1);

				tokens.push_back(token);
				token.clear();
			}
		}
		else
			token += *it;
	}
	if (!token.empty())
	{

		if (token[token.size() - 1] == '\r')
			token.erase(token.size() - 1);

		tokens.push_back(token);
	}
	return tokens;
}

/**
 * Converts a 32-bit integer IP address into its string representation.
 * @param ip_address The 32-bit integer representation of the IP address.
 * @return The string representation of the IP address.
 */
std::string intToIp(uint32_t ip_address)
{
	uint8_t octet1 = ip_address & 0xFF;
	uint8_t octet2 = (ip_address >> 8) & 0xFF;
	uint8_t octet3 = (ip_address >> 16) & 0xFF;
	uint8_t octet4 = (ip_address >> 24) & 0xFF;

	std::ostringstream oss;
	oss << static_cast<int>(octet1) << "." << static_cast<int>(octet2) << "." << static_cast<int>(octet3) << "." << static_cast<int>(octet4);
	return oss.str();
}

/**
 * Converts all characters in a string to uppercase.
 * @param input The string to be converted.
 * @return The input string with all characters in uppercase.
 */
std::string toUpperString(std::string input)
{
	std::string::iterator it;

	for (it = input.begin(); it != input.end(); it++)
		*it = toupper(*it);
	return (input);
}
