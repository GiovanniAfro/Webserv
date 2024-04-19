#include "Socket.hpp"

Socket::Socket(uint16_t port)
: _type("server"), _port(port)
{
	this->_socket = _socketInnit();
	this->_sockAddr.sin_family = AF_INET;
	this->_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_sockAddr.sin_port = htons(this->_port);
	this->_sockAddrLen = sizeof(this->_sockAddr);
	this->_binding();
	this->_listening();
	this->_setNonBlocking();
}

Socket::Socket(int client_socket)
: _type("client"), _socket(client_socket)
{
    this->_sockAddrLen = sizeof(this->_sockAddr);
    // this->_set_non_blocking();
}

Socket::~Socket()
{
	this->closeSocket();
}

int	Socket::_socketInnit()
{
	int	sock;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
        Log::error("Socket creation failed");
		return -1;
	}
	return sock;
}

int	Socket::_binding()
{
    int opt_v = 1;

    // tells the socket to also use addresses that are in the TIME_WAIT state.
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt_v, sizeof(opt_v));

    if (bind(this->_socket, (struct sockaddr *)&this->_sockAddr,
             sizeof(this->_sockAddr)) == -1) {
        Log::error("Binding failed");
        close(this->_socket);
        return -1;;
    }

    return 0;
}

int	Socket::_listening()
{
    if (listen(this->_socket, SOMAXCONN) == -1) {
        Log::error("Listening failed");
        close(this->_socket);
        return -1;;
    }
    std::stringstream	ss;
    ss << ntohs(this->_sockAddr.sin_port);
    Log::info("Server listening on port " + ss.str());

    return 0;
}

int	Socket::_setNonBlocking()
{
    this->_flags = this->getFlags();
    if (fcntl(this->_socket, F_SETFL, this->_flags | O_NONBLOCK) == -1) {
        Log::error("Failed to set socket non-blocking");
        return -1;;
    }
    this->_flags = this->getFlags();

    return 0;
}

const std::string&	Socket::getType()
{ return _type; }

uint16_t	Socket::getPort()
{ return _port; }

int	Socket:: getSocket()
{ return _socket; }

struct sockaddr_in	Socket::getSockAddr()
{ return _sockAddr; }

socklen_t	Socket::getSockAddrLen()
{ return _sockAddrLen; }

int	Socket::getFlags()
{
    int flags = fcntl(this->_socket, F_GETFL, 0);

    if (flags == -1)
	{
        Log::error("Failed to get socket flags");
        exit(EXIT_FAILURE);
    }
    return flags;
}

Socket*	Socket::createClientSocket()
{
    int client_socket = accept(this->_socket, NULL, NULL);

    if (client_socket == -1) {
        Log::error("Acceptance failed");
        close(this->_socket);
        exit(EXIT_FAILURE);
    }
    return new Socket(client_socket);
}

void	Socket::closeSocket()
{ close(_socket); }
