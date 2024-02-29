#pragma once

#include "webserv.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define WHITE "\033[37m"

class Log
{
	private:
		Log();
		Log(const Log &src);
		Log &operator=(const Log &src);
		~Log();

		static std::string _timeStamp();
	public:
		static void info(const std::string &message);
		static void error(const std::string &message);
		static void request(HTTP_METHOD method, const std::string &uri);
};
