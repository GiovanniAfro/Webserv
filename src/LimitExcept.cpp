#include "LimitExcept.hpp"

LimitExcept::LimitExcept()
: ADirective("LimitExcept", GLOBAL_CONTEXT)
{}

LimitExcept::LimitExcept(uint16_t context, enum HTTP_METHOD method)
: ADirective("LimitExcept", context), _method(method)
{}

LimitExcept::LimitExcept(const LimitExcept& copy)
{ *this = copy; }

LimitExcept::~LimitExcept() {}

LimitExcept&	LimitExcept::operator=(const LimitExcept& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_method = other._method;

	return *this;
}

ADirective*	LimitExcept::clone() const
{ return new LimitExcept(*this); }

enum HTTP_METHOD	LimitExcept::getMethod() const
{ return _method; }
