#pragma once

// #include "webserv.h"
#include "ADirective.hpp"

enum HTTP_METHOD {
	GET,
	POST,
	DELETE,
	UNKNOWN
};

enum HTTP_STATUS {
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500
};

struct	Request
{
	std::map<std::string, std::string>	request;
	std::map<std::string, std::string>	requestHeaders;
	std::string 						requestBody;
	enum HTTP_METHOD					requestMethod;
};

class	Http : public ADirective
{
	private:


	public:
		Http();
		Http(const Http& copy);
		~Http();
		Http&	operator=(const Http& other);

		ADirective*	clone() const;

		// Utils -------------------------------------------------------------->
		static enum HTTP_METHOD	_methodToEnum(const std::string& method);
		static enum HTTP_STATUS	_statusToEnum(const std::string& status);
		static std::string		_statusToMessage(enum HTTP_STATUS status);
		static std::string		_statusToString(enum HTTP_STATUS status);

};