#include "webserv.h"

int main(int ac, char **av)
{
	// Create a new Server object
	Server server(8080, 10, (timeval){5, 0});

	// Bind the socket to the address
	if (Error::checkError(server.bind(), FAILED_BIND))
		return (Error::exit(FAILED_BIND, server));

	// Listen for incoming connections
	if (Error::checkError(server.listen(), FAILED_LISTEN))
		return (Error::exit(FAILED_LISTEN, server));

	// Set the socket to non-blocking
	if (Error::checkError(server.nonBlocking(), FAILED_NON_BLOCKING))
		return (Error::exit(FAILED_NON_BLOCKING, server));

	Log::info("Server is running on port " + server.getPortString());

	// The main loop of the server
	return (server.run());
	(void)ac;
	(void)av;
}
