#include "webserv.h"
#include "Server.h"
#include "Error.h"

int main(int ac, char **av)
{
	// Create a new Server object
	Server server(8080);

	// Bind the socket to the address
	if (Error::checkError(server.bind(), "Failed to bind") != NO_ERROR)
		return (Error::closeServer(server));

	// Listen for incoming connections
	int err = server.listen();
	if (err != 0)
	{
		server.close();
		return (1);
	}

	std::cout << "Server is running on port " << server.getPort() << std::endl;
	while (1)
	{
		std::cout << "+++++++ Waiting for new connection ++++++++" << std::endl;

		// Accept a new connection
		err = server.accept();
		if (err < 0)
		{
			std::cout << "Failed to accept connection " << strerror(err) << std::endl;
			continue;
		}

		// Answer the client
		server.answer();

		// Close the client socket
		server.close();
	}

	return (0);
	(void)ac;
	(void)av;
}
