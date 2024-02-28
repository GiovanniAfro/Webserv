#pragma once
#include "webserv.h"
#include "Server.h"
#include <stdio.h>

class Error {
	private:
		Error();
		~Error();
		Error(const Error &src);
		Error &operator=(const Error &src);

	public:
		static ErrorCodes checkError(int ret, std::string message);
		static int closeServer(Server &server);
};
