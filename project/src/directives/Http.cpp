#include "Http.hpp"

Http::Http()
: ADirective("http", GLOBAL_CONTEXT)
{}

Http::Http(const Http& copy)
{ *this = copy; }

Http::~Http()
{}

Http&	Http::operator=(const Http& other)
{
	if (this != &other)
	{
		ADirective::operator=(other);
	}

	return *this;
}

ADirective*	Http::clone() const
{ return new Http(*this); }

enum HTTP_METHOD	Http::_methodToEnum(const std::string& method)
{
	if (method == "GET")
		return GET;
	else if (method == "POST")
		return POST;
	else if (method == "DELETE")
		return DELETE;
	else
		return UNKNOWN;
}

enum HTTP_STATUS	Http::_statusToEnum(const std::string& status)
{
	if (status == "200")
		return OK;
	else if (status == "400")
		return BAD_REQUEST;
	else if (status == "404")
		return NOT_FOUND;
	else
		return INTERNAL_SERVER_ERROR;
}

std::string	Http::_statusToMessage(enum HTTP_STATUS status)
{
	switch (status) {
		case OK:
			return "200 OK";
		case BAD_REQUEST:
			return "400 Bad Request";
		case NOT_FOUND:
			return "404 Not Found";
		default:
			return "500 Internal Server Error";
	}
}

std::string	Http::_statusToString(enum HTTP_STATUS status)
{
	switch (status)
	{
		case OK:
			return "200";
		case BAD_REQUEST:
			return "400";
		case NOT_FOUND:
			return "404";
		default:
			return "500";
	}
}
