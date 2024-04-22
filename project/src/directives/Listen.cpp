/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:44:47 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 14:59:03 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"

Listen::Listen()
: ADirective("listen", SERVER_CONTEXT), _isDefaultServer(false)
{}

Listen::Listen( const std::pair< std::string, std::set<uint16_t> >& addressPort, 
				const std::string& ipAddress, 
				const std::set<uint16_t>& ports, 
				bool isDefault )
: ADirective("listen", SERVER_CONTEXT),_addressPort(addressPort), _ipAddress(ipAddress), _ports(ports), _isDefaultServer(isDefault)
{}

Listen::Listen(const Listen &copy)
: ADirective(copy)
{
	*this = copy;
}

Listen::~Listen() {}

Listen &Listen::operator=(const Listen &other) {
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_addressPort = other._addressPort;
	this->_ipAddress = other._ipAddress;
	this->_ports = other._ports;
	this->_isDefaultServer = other._isDefaultServer;

	return *this;
}

ADirective *Listen::clone() const {
	return new Listen(*this);
}

// void Listen::addAddressPort(std::string &address, uint16_t port) {
// 	_addressPort[address].insert(port);
// }

// void Listen::addAddress(const std::string &address) {
// 	_ipAddress.push_back(address);
// }

void Listen::addPort(uint16_t port) {
	_ports.insert(port);
}

void Listen::setDefaultServer() {
	_isDefaultServer = true;
}

std::pair< std::string, std::set<uint16_t> >&	Listen::getAddressPort() {
	return _addressPort;
}

std::string&	Listen::getAddress() {
	return _ipAddress;
}

std::set<uint16_t>&	Listen::getPorts() {
	return _ports;
}

bool Listen::isDefaultServer() const {
	return _isDefaultServer;
}
