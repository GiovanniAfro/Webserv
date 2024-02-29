#pragma once

#include "webserv.h"

enum ErrorCodes
{
	NO_ERROR = 0,
	FAILED_BIND = 1,
	FAILED_LISTEN = 2,
	FAILED_ACCEPT = 3,
	FAILED_SEND = 4,
	FAILED_RECEIVE = 5,
	FAILED_CLOSE = 6,
	FAILED_BLOCK = 7
};

/*!
* @brief Error handling class
* @details This class handles errors and exits the program if an error occurs.
*/
class Error {
	private:
		Error();
		~Error();
		Error(const Error &src);
		Error &operator=(const Error &src);

	public:
		static ErrorCodes checkError(int ret, ErrorCodes exit);
		static int exit(ErrorCodes exit, Server &server);
};
