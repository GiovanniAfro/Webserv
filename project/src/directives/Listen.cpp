/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:44:47 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/06 15:56:15 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"

Listen::Listen()
: ADirective("listen", SERVER_CONTEXT), _isDefaultServer(false)
{}

Listen::Listen( const std::pair< std::string, uint16_t>& addressPort, 
				const std::string& ipAddress, 
				uint16_t port, 
				bool isDefault )
: ADirective("listen", SERVER_CONTEXT),_addressPort(addressPort), _ipAddress(ipAddress), _port(port), _isDefaultServer(isDefault)
{}

Listen::Listen(const Listen &copy)
: ADirective(copy)
{ *this = copy; }

Listen::~Listen() {}

Listen &Listen::operator=(const Listen &other)
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

ADirective *Listen::clone() const
{ return new Listen(*this); }

std::pair< std::string, uint16_t>&	Listen::getAddressPort()
{ return _addressPort; }

std::string&	Listen::getAddress()
{ return _ipAddress; }

uint16_t	Listen::getPort()
{ return _port; }

bool Listen::isDefaultServer() const
{ return _isDefaultServer; }
