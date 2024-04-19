/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:44:51 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/18 22:25:00 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#location
    Syntax:	    location [ = | ~ | ~* | ^~ ] uri { ... }
                location @name { ... }
    Default:	—
    Context:	server, location
 */
class	Location : public ADirective
{
	private:


		Location();

	public:
		Location(uint16_t context);
		Location(const Location& copy);
		~Location();
		Location&	operator=(const Location& other);

		ADirective*	clone() const;



};
