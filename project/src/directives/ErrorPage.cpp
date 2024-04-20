#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
: ADirective("ErrorPage", GLOBAL_CONTEXT), _response(NOT_FOUND)
{}

ErrorPage::ErrorPage(uint16_t context, const std::vector<enum HTTP_STATUS>& codes, enum HTTP_STATUS response, const std::string& uri)
: ADirective("ErrorPage", context), _codes(codes), _response(response), _uri(uri)
{}

ErrorPage::ErrorPage(const ErrorPage& copy)
{ *this = copy; }

ErrorPage::~ErrorPage() {}

ErrorPage&	ErrorPage::operator=(const ErrorPage& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_codes = other._codes;
	this->_response = other._response;
	this->_uri = other._uri;

	return *this;
}

ADirective*	ErrorPage::clone() const
{ return new ErrorPage(*this); }

// size_t	ErrorPage::getSize() const
// { return _size; }
