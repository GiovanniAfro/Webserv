#pragma once

#include "webserv.h"

#define PORT 4242

/*!
* @brief A class to represent a server
* @param _socket The file descriptor of the socket
* @param _clientSocket The file descriptor of the client socket
* @param _address The address information of the server, includes:
* 	- sin_family: The protocol family to be used, in this case, the Internet Protocol v4 addresses.
* 	- sin_port: The port number of the server
* 	- sin_addr: The IPv4 address of the server
*/
class Server {
	private:
		int _socket;
		int _clientSocket;
		sockaddr_in _address;

		Server(const Server &src);
		Server &operator=(const Server &src);
	public:
		Server();
		~Server();

		int bind();
		void listen();
		void accept();
		void answer();
		void close();
		int getSocket() const;
		sockaddr_in getAddress() const;
};
