#include "Include.hpp"

Include::Include()
: ADirective("include", HTTP_CONTEXT)
{}

Include::Include(const std::string& path)
: ADirective("include", HTTP_CONTEXT)
{
	std::ifstream		config;

	config.open(path.c_str());
	if (!config.is_open())
		throw std::runtime_error("webserv: Include: file does not exist");
	config.close();
	_path = path;
}

Include::Include(const Include& copy)
{ *this = copy; }

Include::~Include() {}

Include&	Include::operator=(const Include& other)
{
	if (this == &other)
		return *this;
	
	ADirective::operator=(other);
	this->_path = other._path;

	return *this;
}

ADirective*	Include::clone() const
{ return new Include(*this); }

const std::string	Include::getPath() const
{ return _path; }
