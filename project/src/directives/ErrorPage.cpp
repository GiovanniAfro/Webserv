#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
: ADirective("ErrorPage", GLOBAL_CONTEXT)
{}

ErrorPage::ErrorPage(uint16_t context)
: ADirective("ErrorPage", context)
{}

ErrorPage::ErrorPage(const ErrorPage& copy)
{ *this = copy; }

ErrorPage::~ErrorPage() {}

ErrorPage&	ErrorPage::operator=(const ErrorPage& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);


	return *this;
}

ADirective*	ErrorPage::clone() const
{ return new ErrorPage(*this); }

// size_t	ErrorPage::getSize() const
// { return _size; }
