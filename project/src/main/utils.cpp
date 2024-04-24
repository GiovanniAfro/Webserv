/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:48:46 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 12:16:05 by adi-nata         ###   ########.fr       */
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

bool isServerDirective(const std::string &s)
{
	const std::string	directiveNames[] = {"listen", "root", "server_name", "index", "error_page", "location"};

	for (size_t i = 0; i < directiveNames->size(); ++i) {
		if (s == directiveNames[i])
			return true;
	}

	return false;
}

// bool	isContextDirective(const std::string& directiveName, uint16_t context)
// {



// }

bool isDirective(const std::string &s)
{
	const std::string	directiveNames[] = {"http", "include", "server", "listen", "root", "server_name", "index", "error_page", "location", "autoindex", "limit_except"};

	for (unsigned int i = 0; i < NUM_DIRECTIVES; ++i) {
		if (s == directiveNames[i])
			return true;
	}

	return false;
}

std::vector<uint16_t> getContextValues(int directive) {
	switch (directive) {
		case HTTP_DIRECTIVE:
			return std::vector<uint16_t>(1, GLOBAL_CONTEXT);

		case INCLUDE_DIRECTIVE:
			return std::vector<uint16_t>(1, GLOBAL_CONTEXT);

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

		default:
			return std::vector<uint16_t>();
	}
}

// get index's directive context and compare with current context
						  // http			// global	
bool checkContext(int directiveContext, uint16_t currentContext)
{
	std::vector<uint16_t>	contextLevels = getContextValues(directiveContext);

	for (std::vector<uint16_t>::iterator it = contextLevels.begin(); it != contextLevels.end(); ++it) {
		if ((*it) <= currentContext)
			return true;
	}

	return false;
}

int	whichDirective(const std::string &s)
{
	const std::string	directiveNames[] = {"http", "include", "server", "listen", "root", "server_name", "index", "error_page", "location", "autoindex", "limit_except"};

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

std::string	secondToken(const std::string &str) {
	// istd::stringstream	iss(str);
	// std::string			token;

	// if (!(iss >> token) || !(iss >> token))
	// 	return "";
	// if (token.empty() || token.back() != ';')
	// 	return "";
	// return token.substr(0, token.size() - 1);

	size_t	start = firstToken(str).length();
	if (!start)
		return "";
	while (str[start] && (str[start] == ' ' || str[start] == '\t'))
		start++;

	size_t	end = str.find_first_of(";{");
	if (end == std::string::npos)
		return "";

	return str.substr(start, end - start);
}

// bool	checkBrackets()
// {

// }
