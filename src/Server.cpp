#include "Server.h"
#include <string.h>

/*!
* @brief Initialize a new Server object, and create a new socket
* @details This constructor initializes a new Server object, and creates a new socket using
	the socket() function from the sys/socket.h library by giving it three arguments:
	- AF_INET: The protocol family to be used, in this case, the Internet Protocol v4 addresses.
	- SOCK_STREAM: The type of socket to be created, in this case, a stream socket.
	- 0: The protocol to be used, in this case, the default protocol for the given protocol family and socket type.

	Then, it initializes the _address attribute with the following values:
	- sin_family: The protocol family to be used, in this case, the Internet Protocol v4 addresses.
	- sin_addr: The IPv4 address of the server, not bound to any specific address, will listen to all available interfaces, using the INADDR_ANY macro from the netinet/in.h library.
	- sin_port: The port number of the server, converted to network byte order using the htons() function from the netinet/in.h library.
*/
Server::Server() : _port(PORT)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);
}

Server::Server(uint16_t port) : _port(port)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);
}

/*!
* @brief Delete a Server object
* @details This destructor deletes a Server object, and closes the socket using the close() function from the unistd.h library.
*/
Server::~Server()
{
	close();
}

/*!
* @brief Bind the socket to the address
* @details This function binds the socket to the address using the bind() function from the sys/socket.h library.
	- The first argument is the file descriptor of the socket.
	- The second argument is the address information of the server.
	- The third argument is the size of the address information of the server.
* @return The result of the bind() function
*/
int Server::bind()
{
	// memset(_serverAddress.sin_zero, '\0', sizeof _serverAddress.sin_zero);
	return (::bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)));
}

/*!
* @brief Listen for incoming connections
* @param connections The maximum length of the queue of pending connections
* @details This function listens for incoming connections using the listen() function from the sys/socket.h library.
	- The first argument is the file descriptor of the socket.
	- The second argument is the maximum length of the queue of pending connections.
* @return The result of the listen() function
*/
int Server::listen(int connections)
{
	return (::listen(_serverSocket, connections));
}

/*!
* @brief Accept a new connection
* @details This function accepts a new connection using the accept() function from the sys/socket.h library.
	- The first argument is the file descriptor of the socket.
	- The second argument is the ad dress information of the client.
	- The third argument is the size of the address information of the client.
*/
int Server::accept()
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	_clientSocket = ::accept(_serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
	return (_clientSocket);
}

/*!
* @brief Answer the client
* @details This function answers the client using the send() function from the sys/socket.h library.
	- The first argument is the file descriptor of the client socket.
	- The second argument is the message to be sent.
	- The third argument is the size of the message to be sent.
*/
void Server::answer()
{
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
	send(_clientSocket, response.c_str(), response.size(), 0);
}

/*!
* @brief Close the client socket
* @details This function closes the client socket using the close() function from the unistd.h library.
*/
void Server::close()
{
	if (_clientSocket > 0)
		::close(_clientSocket);
}

/*!
* @brief Get the file descriptor of the socket
* @return The file descriptor of the socket
*/
int Server::getSocket() const
{
	return (_serverSocket);
}

/*!
* @brief Get the port number of the server
* @return The port number of the server
*/
uint16_t Server::getPort() const
{
	return (_port);
}

std::string Server::getPortString() const
{
	std::stringstream ss;
	ss << _port;
	return (ss.str());
}

/*!
* @brief Get the address information of the server
* @return The address information of the server
*/
sockaddr_in Server::getAddress() const
{
	return (_serverAddress);
}
