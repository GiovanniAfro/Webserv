/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySize.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:24 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/18 22:07:15 by adi-nata         ###   ########.fr       */
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

class	ClientMaxBodySize : public ADirective
{
	private:
		size_t	_size;

		ClientMaxBodySize();

	public:
		ClientMaxBodySize(uint16_t context, size_t size);
		ClientMaxBodySize(const ClientMaxBodySize& copy);
		~ClientMaxBodySize();
		ClientMaxBodySize&	operator=(const ClientMaxBodySize& other);

		ADirective*	clone() const;

		size_t	getSize() const;

};
