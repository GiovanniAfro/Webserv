/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:45:25 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/23 17:48:01 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Module:     https://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except
    Syntax:	    limit_except method ... { ... }
    Default:	—
    Context:	location
 */
class	LimitExcept : public ADirective
{
	private:
		enum HTTP_METHOD	_method;

		LimitExcept();

	public:
		LimitExcept(uint16_t context, enum HTTP_METHOD method);
		LimitExcept(const LimitExcept& copy);
		~LimitExcept();
		LimitExcept&	operator=(const LimitExcept& other);

		ADirective*	clone() const;

		enum HTTP_METHOD getMethod() const;
};
