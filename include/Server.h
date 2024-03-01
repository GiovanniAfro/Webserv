#pragma once

#include "webserv.h"

// Macros for the server default arguemnts, in case the configuration file is not provided
#define PORT 4242
#define MAX_CONNECTIONS 10
#define TIMEOUT_SEC 5
#define TIMEOUT_USEC 0
#define BUFFER_SIZE 1024

/*!
* @brief A class to represent a server
* @param _port The port number of the server
* @param _serverSocket The file descriptor of the socket
* @param _serverAddress The address information of the server, includes:
* 	- sin_family: The protocol family to be used, in this case, the Internet Protocol v4 addresses.
* 	- sin_port: The port number of the server
* 	- sin_addr: The IPv4 address of the server
* @param _clientSocket The file descriptor of the client socket
* @param _flags The flags of the blocking mode of the socket
*/
class Server {
	private:
		// From the configuration file
		const int _port;
		const int _maxConnections;
		const timeval _timeout;

		// Socket information
		int _serverSocket;
		sockaddr_in _serverAddress;
		int _clientSocket;
		int _flags;

		Server(const Server &src);
		Server &operator=(const Server &src);

		// Server runtime functions
		void _clientRequest();
		int _accept();
		void _answer();
	public:
		Server();
		Server(uint16_t port, int maxConnections, timeval timeout);
		~Server();

		// Server setup functions
		int bind();
		int listen(int connections = MAX_CONNECTIONS);
		int nonBlocking();

		// Server runtime functions
		int run();
		void close();

		// Getters
		uint16_t getPort() const;
		std::string getPortString() const;
		int getSocket() const;
		sockaddr_in getAddress() const;
};
