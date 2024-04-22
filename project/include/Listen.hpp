/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:34:12 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 15:00:10 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

class ADirective;

// Class ---------------------------------------------------------------------->

/*!
 * @ref
	Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
	Syntax:	    listen address[:port] [default_server];
				listen port [default_server];
				listen unix:path [default_server];
	Default:	listen *:80 | *:8000;
	Context:	server
 */
class Listen : public ADirective {
	private:
		std::pair< std::string, std::set<uint16_t> >	_addressPort;	// In the context of a listen directive, it's more common to have multiple ports per IP address
		std::string										_ipAddress;		// if ipAddress, addressPort instead of port
		std::set<uint16_t>								_ports;
		bool											_isDefaultServer;


	public:
		Listen();
		Listen( const std::pair< std::string, std::set<uint16_t> >& addressPort, 
				const std::string& ipAddress, 
				const std::set<uint16_t>&	ports, 
				bool isDefault );
		Listen(const Listen &copy);
		~Listen();
		Listen &operator=(const Listen &other);

		ADirective *clone() const;

		void    addAddressPort(std::string &address, uint16_t port);
		void    addAddress(const std::string &address);
		void    addPort(uint16_t port);
		void    setDefaultServer();

		std::pair< std::string, std::set<uint16_t> >&	getAddressPort();
		std::string&									getAddress();
		std::set<uint16_t>&								getPorts();
		bool											isDefaultServer() const;

};
