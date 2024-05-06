/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:48:46 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/06 14:53:31 by gcavanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

std::string	strip(const std::string &str) {
	size_t first, last;

	first = str.find_first_not_of(" \t\n\r");
	last = str.find_last_not_of(" \t\n\r");
	if (first == std::string::npos)
		return "";
	return str.substr(first, last - first + 1);
}

bool isClosedBracket(const std::string &s) {
	std::stringstream	iss(s);
	std::string			token;

	if (iss >> token && token == "}")
		return true;

	return false;
}

bool isBracket(const std::string &s) {
	std::stringstream	iss(s);
	std::string			token;

	if (iss >> token && strchr("{}", token[0]))
		return true;

	return false;
}

bool isComment(const std::string &s) {
	std::stringstream	iss(s);
	std::string			token;

	if (iss >> token && token[0] == '#')
		return true;

	return false;
}

bool isDirective(const std::string &s)
{
	const std::string	directiveNames[] = {"http", "include", "server", "listen", "root", "server_name", "index", "error_page", "location", "autoindex", "limit_except", "client_max_body_size", "alias", "rewrite", "fastcgi_pass"};

	for (unsigned int i = 0; i < NUM_DIRECTIVES; ++i) {
		if (s == directiveNames[i])
			return true;
	}

	return false;
}

bool	isIPAddress(const std::string& s)
{
	struct in_addr	addr;
	if (inet_pton(AF_INET, s.c_str(), &addr) == 1)
		return true;
	return false;
}

std::vector<uint16_t> getContextValues(int directive) {
	switch (directive) {
		case HTTP_DIRECTIVE:
			return std::vector<uint16_t>(1, GLOBAL_CONTEXT);

		case INCLUDE_DIRECTIVE:
			return std::vector<uint16_t>(1, HTTP_CONTEXT);

		case SERVER_DIRECTIVE:
			return std::vector<uint16_t>(1, HTTP_CONTEXT);

		case LISTEN_DIRECTIVE:
			return std::vector<uint16_t>(1, SERVER_CONTEXT);

		case ROOT_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(HTTP_CONTEXT);
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case SERVER_NAME_DIRECTIVE:
			return std::vector<uint16_t>(1, SERVER_CONTEXT);

		case INDEX_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(HTTP_CONTEXT);
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case ERRORPAGE_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(HTTP_CONTEXT);
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case LOCATION_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case AUTOINDEX_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(HTTP_CONTEXT);
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case LIMITEXCEPT_DIRECTIVE:
			return std::vector<uint16_t>(1, LOCATION_CONTEXT);

		case CLIENTMAXBODYSIZE_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(HTTP_CONTEXT);
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case ALIAS_DIRECTIVE:
			return std::vector<uint16_t>(1, LOCATION_CONTEXT);

		case REWRITE_DIRECTIVE:
		{
			std::vector<uint16_t> values;
			values.push_back(SERVER_CONTEXT);
			values.push_back(LOCATION_CONTEXT);
			return values;
		}

		case FASTCGIPASS_DIRECTIVE:
			return std::vector<uint16_t>(1, LOCATION_CONTEXT);
		default:
			return std::vector<uint16_t>();
	}
}

bool checkContext(int directiveContext, uint16_t currentContext)
{
	std::vector<uint16_t>	contextLevels = getContextValues(directiveContext);

	for (std::vector<uint16_t>::iterator it = contextLevels.begin(); it != contextLevels.end(); ++it)
	{
		if ((*it) == currentContext)
			return true;
	}

	return false;
}

int	whichDirective(const std::string &s)
{
	const std::string	directiveNames[] = {"http", "include", "server", "listen", "root", "server_name", "index", "error_page", "location", "autoindex", "limit_except", "client_max_body_size", "alias", "rewrite", "fastcgi_pass"};

	for (int i = 0; i < NUM_DIRECTIVES; ++i) {

		if (s == directiveNames[i])
			return i;
	}

	return -1;
}

std::string	firstToken(const std::string &str) {
	std::stringstream	iss(str);
	std::string			token;

	if (!(iss >> token))
		return "";
	if (isDirective(token))
		return token;

	return "";
}

std::string	secondToken(const std::string& str)
{
	size_t	start = firstToken(str).length();
	if (!start)
		return "";
	while (str[start] && (str[start] == ' ' || str[start] == '\t'))
		start++;

	size_t	end = str.find_first_of(";{");	// + '\n' for context directives ?
	if (end == std::string::npos)
		return "";

	// if (str[end] == '{')
	// {
	// 	size_t	whiteSpace = str.find_first_of(" \t", start);
	// 	if (whiteSpace != std::string::npos && whiteSpace < end)
	// 		end = whiteSpace;
	// }

	return strip(str.substr(start, end - start));
}

bool	isLocationModifier(const std::string& s)
{
	enum LOCATION_MODIFIER	modifier = Location::_modifierToEnum(s);

	if (modifier != INVALID)
		return true;

	return false;
}

// bool	checkBrackets()
// {

// }

/*!
	* @brief Check if the path is a folder.
	* @param path Path to the file.
	* @return bool True if the path is a folder, false otherwise.
*/
bool	isFolder(const std::string &path)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) == 0)
		return S_ISDIR(buffer.st_mode);
	return false;
}

/*!
	* @brief Check if the path is a file.
	* @param path Path to the file.
	* @return bool True if the path is a file, false otherwise.
*/
bool	isFile(const std::string &path) {
	struct stat buffer;

	if (stat(path.c_str(), &buffer) == 0)
		return S_ISREG(buffer.st_mode);
	return false;
}

std::string	strRemove(std::string const &str, std::string const &toRemove)
{
	std::string newStr = "";
	std::cout << "Removing " << toRemove << std::endl;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (str.substr(it - str.begin(), toRemove.size()) == toRemove)
		{
			it += toRemove.size() - 1;
		}
		else
			newStr += *it;
	}
	return newStr;
}

std::string	strReplace(std::string const &str, std::string const &from, std::string const &to)
{
	std::string newStr = "";
	std::cout << "Replacing " << from << " with " << to << std::endl;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (str.substr(it - str.begin(), from.size()) == from)
		{
			newStr += to;
			it += from.size() - 1;
		}
		else
			newStr += *it;
	}
	std::cout << "New string: " << newStr << std::endl;
	return newStr;
}

void split_path(const std::string &full_path, std::string &directory, std::string &filename) {
    size_t found = full_path.find_last_of("/\\");
    if (found != std::string::npos) {
        directory = full_path.substr(0, found);
        filename = full_path.substr(found + 1);
    } else {
        directory = "";
        filename = full_path;
    }
}
