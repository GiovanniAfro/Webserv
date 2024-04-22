#include "Index.hpp"

Index::Index(const std::string& content, uint16_t context)
: ADirective("index", context)
{
	std::istringstream	iss(content);
	std::string			token;

	if (content.empty())
		throw std::runtime_error("index : empty content");

	while (iss >> token)
	{
		// check _files's content? ...
		_files.push_back(token);
	}
	if (_files.back()[0] == '/')	// isListenModifier()?
	{
		// expand
		;
	}

}

Index::Index(const Index& copy) : ADirective(copy)
{ *this = copy; }

Index::~Index() {}

Index&	Index::operator=(const Index& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_files = other._files;
	this->_lastElement = other._lastElement;

	return *this;
}

ADirective*	Index::clone() const
{ return new Index(*this); }

std::vector<std::string>&	Index::getFiles()
{ return _files; }

std::string&	Index::getLastElement()
{ return _lastElement; }
