/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:34:12 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/06 15:55:32 by adi-nata         ###   ########.fr       */
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
class Listen : public ADirective
{
	private:
		std::pair< std::string, uint16_t>	_addressPort;	// In the context of a listen directive, it's more common to have multiple ports per IP address
		std::string							_ipAddress;		// if ipAddress, addressPort instead of port
		uint16_t							_port;
		bool								_isDefaultServer;

		Listen();

	public:
		Listen( const std::pair< std::string, uint16_t>& addressPort, 
				const std::string& ipAddress, 
				uint16_t ports, 
				bool isDefault );
		Listen(const Listen &copy);
		~Listen();
		Listen &operator=(const Listen &other);

		ADirective *clone() const;

		std::pair< std::string, uint16_t>&	getAddressPort();
		std::string&						getAddress();
		uint16_t							getPort();
		bool								isDefaultServer() const;

};
