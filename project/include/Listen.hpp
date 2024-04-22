/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:34:12 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:36:59 by kichkiro         ###   ########.fr       */
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
		std::map< std::string, std::set<uint16_t> >	_addressPort;	// In the context of a listen directive, it's more common to have multiple ports per IP address
		std::vector<std::string>					_ipAddress;		// if ipAddress, addressPort instead of port
		std::set<uint16_t>							_port;
		bool										_isDefaultServer;

	public:
		Listen();
		// Listen();
		Listen(const Listen &copy);
		~Listen();
		Listen &operator=(const Listen &other);

		ADirective *clone() const;

		void    addAddressPort(std::string &address, uint16_t port);
		void    addAddress(const std::string &address);
		void    addPort(uint16_t port);
		void    setDefaultServer();

		std::map< std::string, std::set<uint16_t> > &getAddressPort();
		std::vector<std::string> &getAddress();
		std::set<uint16_t> &getPorts();
		bool isDefaultServer() const;

};
