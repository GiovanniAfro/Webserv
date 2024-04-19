/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:19 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/18 22:18:35 by adi-nata         ###   ########.fr       */
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
		// To define ...

		ErrorPage();

	public:
		ErrorPage(uint16_t context);
		ErrorPage(const ErrorPage& copy);
		~ErrorPage();
		ErrorPage&	operator=(const ErrorPage& other);

		ADirective*	clone() const;

		// size_t	getSize() const;

};
