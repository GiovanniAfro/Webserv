#include "webserv.h"

int main(int ac, char **av)
{
	// Create a new Server object
	Server server(8080);

	// Bind the socket to the address
	if (Error::checkError(server.bind(), FAILED_BIND))
		return (Error::exit(FAILED_BIND, server));

	// Listen for incoming connections
	if (Error::checkError(server.listen(), FAILED_LISTEN))
		return (Error::exit(FAILED_LISTEN, server));

	Log::info("Server is running on port " + server.getPortString());

	while (1)
	{
		// Accept a new connection
		if (Error::checkError(server.accept(), FAILED_ACCEPT))
			continue;
		Log::request(GET, "/");

		// Answer the client
		server.answer();

		// Close the client socket
		server.close();
	}

	return (0);
	(void)ac;
	(void)av;
}
