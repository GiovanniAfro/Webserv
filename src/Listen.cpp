#include "Listen.hpp"

Listen::Listen()
: ADirective("listen", SERVER_CONTEXT)
{}

Listen::Listen(const Listen& copy)
{ *this = copy; }

Listen::~Listen() {}

Listen&	Listen::operator=(const Listen& other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_addressPort = other._addressPort;
	this->_ipAddress = other._ipAddress;
	this->_port = other._port;
	this->_isDefaultServer = other._isDefaultServer;

	return *this;
}

ADirective*	Listen::clone() const
{ return new Listen(*this); }


void	Listen::addAddressPort(std::string& address, uint16_t port)
{ _addressPort[address].insert(port);}

void	Listen:: addAddress(const std::string& address)
{ _ipAddress.push_back(address); }

void	Listen:: addPort(uint16_t port)
{ _port.insert(port); }

void	Listen::setDefaultServer()
{ _isDefaultServer = true; }


std::map< std::string, std::set<uint16_t> >&	Listen::getAddressPort()
{ return _addressPort; }

std::vector<std::string>&	Listen:: getAddress()
{ return _ipAddress; }

std::set<uint16_t>&	Listen::getPorts()
{ return _port; }

bool	Listen::isDefaultServer() const
{ return _isDefaultServer; }
