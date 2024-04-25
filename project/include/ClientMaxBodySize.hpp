/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySize.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:24 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/25 11:56:44 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
    Syntax:	    client_max_body_size size;
    Default:	client_max_body_size 1m;
    Context:	http, server, location
 */
class ClientMaxBodySize : public ADirective
{
	private:
		unsigned long long	_size;

		ClientMaxBodySize();

	public:
		ClientMaxBodySize(uint16_t context, unsigned long long size);
		ClientMaxBodySize(const ClientMaxBodySize& copy);
		~ClientMaxBodySize();
		ClientMaxBodySize&	operator=(const ClientMaxBodySize& other);

		ADirective*	clone() const;

		unsigned long long	getSize() const;
};
