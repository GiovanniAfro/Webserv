/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:33:13 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:35:44 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "webserv.h"
#include "ADirective.hpp"

enum HTTP_METHOD {
	GET,
	POST,
	DELETE,
	UNKNOWN
};
extern std::vector<HTTP_METHOD>	allHttpMethods;

enum HTTP_STATUS {
	OK = 200,
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	REQUEST_TIMEOUT = 408,
	PAYLOAD_TOO_LARGE = 413,
	REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	INTERNAL_SERVER_ERROR = 500
};
extern std::vector<HTTP_STATUS>	allHttpStatus;

struct	Request {
	std::map<std::string, std::string>	request;
	std::map<std::string, std::string>	requestHeaders;
	std::string 						requestBody;
	enum HTTP_METHOD					requestMethod;
};

class Http : public ADirective {
	private:


	public:
		Http();
		Http(const Http &copy);
		~Http();
		Http &operator=(const Http &other);

		ADirective *clone() const;

		// Utils -------------------------------------------------------------->
		static enum HTTP_METHOD	_methodToEnum(const std::string &method);
		static enum HTTP_STATUS	_statusToEnum(const std::string &status);
		static std::string		_statusToMessage(enum HTTP_STATUS status);
		static std::string		_statusToString(enum HTTP_STATUS status);
};
