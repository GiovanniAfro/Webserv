/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rewrite.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:00 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/18 22:29:37 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
	Docs:       https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#rewrite
	Syntax:     rewrite regex replacement [flag];
	Default:	—
	Context:	server, location, if
 */
class	Rewrite : public ADirective
{
	private:


		Rewrite();

	public:
		Rewrite(uint16_t context);
		Rewrite(const Rewrite& copy);
		~Rewrite();
		Rewrite&	operator=(const Rewrite& other);

		ADirective*	clone() const;



};