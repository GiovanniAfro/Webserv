#include "ClientMaxBodySize.hpp"

ClientMaxBodySize::ClientMaxBodySize()
: ADirective("ClientMaxBodySize", GLOBAL_CONTEXT)
{}

ClientMaxBodySize::ClientMaxBodySize(uint16_t context, size_t size)
: ADirective("ClientMaxBodySize", context), _size(size)
{}

ClientMaxBodySize::ClientMaxBodySize(const ClientMaxBodySize& copy)
{ *this = copy; }

ClientMaxBodySize::~ClientMaxBodySize() {}

ClientMaxBodySize&	ClientMaxBodySize::operator=(const ClientMaxBodySize& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_size = other._size;

	return *this;
}

ADirective*	ClientMaxBodySize::clone() const
{ return new ClientMaxBodySize(*this); }

size_t	ClientMaxBodySize::getSize() const
{ return _size; }
