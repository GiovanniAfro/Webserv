#include "Alias.hpp"

Alias::Alias()
: ADirective("Alias", GLOBAL_CONTEXT)
{}

Alias::Alias(uint16_t context, const std::string& path)
: ADirective("Alias", context), _path(path)
{}

Alias::Alias(const Alias& copy)
{ *this = copy; }

Alias::~Alias() {}

Alias&	Alias::operator=(const Alias& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_path = other._path;

	return *this;
}

ADirective*	Alias::clone() const
{ return new Alias(*this); }

const std::string&	Alias::getPath() const
{ return _path; }
