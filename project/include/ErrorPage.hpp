/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:19 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/21 02:03:12 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page
    Syntax:	    error_page code ... [=[response]] uri;
    Default:	—
    Context:	http, server, location
 */
class	ErrorPage : public ADirective
{
	private:
		std::vector<enum HTTP_STATUS>	_codes;
		enum HTTP_STATUS				_response;
		std::string						_uri;

		ErrorPage();

	public:
		ErrorPage(uint16_t context, const std::vector<enum HTTP_STATUS>& codes, enum HTTP_STATUS response, const std::string& uri);
		ErrorPage(const ErrorPage& copy);
		~ErrorPage();
		ErrorPage&	operator=(const ErrorPage& other);

		ADirective*	clone() const;

		std::vector<enum HTTP_STATUS>&	getCodes();
		enum HTTP_STATUS				getResponse();
		const std::string&				getUri() const;
		

};
