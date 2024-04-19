#include "ServerName.hpp"

ServerName::ServerName(const std::string& content)
{
	std::stringstream	iss(content);
	std::string			token;

	while (iss >> token)
		_names.push_back(token);
}

ServerName::ServerName(const ServerName& copy)
{ *this = copy; }

ServerName::~ServerName() {}

ServerName&	ServerName::operator=(const ServerName& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_names = other._names;

	return *this;
}

ADirective*	ServerName::clone() const
{ return new ServerName(*this); }

std::vector<std::string>&	ServerName::getNames()
{ return _names; }
