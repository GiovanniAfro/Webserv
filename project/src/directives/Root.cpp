#include "Root.hpp"

Root::Root()
: ADirective("root", GLOBAL_CONTEXT)
{}

Root::Root(uint16_t context, const std::string& path)
: ADirective("root", context), _path(path)
{
	std::stringstream	iss(path);
	std::string			token;
	unsigned int		tokenCount = 0;

	if (path.empty())
		throw std::runtime_error("root : empty content");
	while (iss >> token)
		++tokenCount;
	if (tokenCount != 1)
		throw std::runtime_error("root : invalid content");

	// struct stat	info;
	// if (stat(token.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR))
	// 	throw std::runtime_error("root : invalid path");
}

Root::Root(const Root& copy)
{ *this = copy; }

Root::~Root() {}

Root&	Root::operator=(const Root& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_path = other._path;

	return *this;
}

ADirective*	Root::clone() const
{ return new Root(*this); }

const std::string&	Root::getPath() const
{ return _path; }
