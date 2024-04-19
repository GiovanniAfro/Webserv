#include "Rewrite.hpp"

Rewrite::Rewrite()
: ADirective("Rewrite", GLOBAL_CONTEXT)
{}

Rewrite::Rewrite(uint16_t context)
: ADirective("Rewrite", context)
{}

Rewrite::Rewrite(const Rewrite& copy)
{ *this = copy; }

Rewrite::~Rewrite() {}

Rewrite&	Rewrite::operator=(const Rewrite& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);


	return *this;
}

ADirective*	Rewrite::clone() const
{ return new Rewrite(*this); }


