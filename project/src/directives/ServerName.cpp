/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerName.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:48:07 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/05 20:35:25 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerName.hpp"

ServerName::ServerName(const std::string &content)
: ADirective("server_name", SERVER_CONTEXT)
{
	std::stringstream	iss(content);
	std::string			token;

	if (content.empty())
		throw std::runtime_error("server_name : empty content");

	while (iss >> token)
		_names.push_back(token);

	// check _names's content? ...
}

ServerName::ServerName(const ServerName &copy) : ADirective(copy)
{ *this = copy; }

ServerName::~ServerName() {}

ServerName &ServerName::operator=(const ServerName &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_names = other._names;

	return *this;
}

ADirective *ServerName::clone() const
{ return new ServerName(*this); }

std::vector<std::string> &ServerName::getNames()
{ return _names; }
