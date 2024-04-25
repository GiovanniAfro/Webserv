/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rewrite.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:00 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/25 17:30:53 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
	Docs:       https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#rewrite
	Syntax:     rewrite regex replacement [flag];
	Syntax:     rewrite uri replacement [redirect];
	Default:	—
	Context:	server, location, if
 */
class Rewrite : public ADirective
{
	private:
		std::string	_uri, _replacement;
		bool		_redirect;

		Rewrite();

	public:
		Rewrite(uint16_t context, const std::string& uri, const std::string& replacement, bool redirect);
		Rewrite(const Rewrite& copy);
		~Rewrite();
		Rewrite&	operator=(const Rewrite& other);
		ADirective*	clone() const;

		const std::string&	getUri() const;
		const std::string&	getReplacement() const;
		bool				isRedirect() const;
};
