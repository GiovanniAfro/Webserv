#include "Error.h"

ErrorCodes Error::checkError(int ret, std::string message)
{
	if (ret < 0)
	{
		std::cout << strerror(ret) << std::endl;
		return (FAILED_BIND);
	}
	return (NO_ERROR);
}

int Error::closeServer(Server &server)
{
	server.close();
	return (1);
}
