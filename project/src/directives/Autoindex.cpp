#include "Autoindex.hpp"

Autoindex::Autoindex()
: ADirective("Autoindex", GLOBAL_CONTEXT)
{}

Autoindex::Autoindex(uint16_t context, bool isEnabled)
: ADirective("Autoindex", context), _isEnabled(isEnabled)
{}

Autoindex::Autoindex(const Autoindex& copy)
{ *this = copy; }

Autoindex::~Autoindex() {}

Autoindex&	Autoindex::operator=(const Autoindex& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_isEnabled = other._isEnabled;

	return *this;
}

ADirective*	Autoindex::clone() const
{ return new Autoindex(*this); }

bool	Autoindex::getMode() const
{ return _isEnabled; }
