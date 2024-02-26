#include "webserv.h"
#include "Server.h"

int main(int ac, char **av)
{
	// Create a new Server object
	Server server;

	// Bind the socket to the address
	server.bind();

	while (1)
	{
		// Listen for incoming connections
		server.listen();

		// Accept a new connection
		server.accept();

		// Answer the client
		server.answer();

		// Close the client socket
		server.close();
	}

	return (0);
	(void)ac;
	(void)av;
}
