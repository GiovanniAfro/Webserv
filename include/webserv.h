#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

enum ErrorCodes
{
	NO_ERROR = 0,
	FAILED_BIND = 1,
	FAILED_LISTEN = 2,
	FAILED_ACCEPT = 3,
	FAILED_SEND = 4,
	FAILED_RECEIVE = 5,
	FAILED_CLOSE = 6
};
