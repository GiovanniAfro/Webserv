#include "Location.hpp"

Location::Location()
: ADirective("Location", GLOBAL_CONTEXT)
{}

Location::Location(uint16_t context)
: ADirective("Location", context)
{}

Location::Location(const Location& copy)
{ *this = copy; }

Location::~Location() {}

Location&	Location::operator=(const Location& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);


	return *this;
}

ADirective*	Location::clone() const
{ return new Location(*this); }


