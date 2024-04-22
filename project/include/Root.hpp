/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Root.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:35:19 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:37:17 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

class	ADirective;

/*!
 * @ref 
	Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#root
	Syntax:	    root path;
	Default:	root html;
	Context:	http, server, location, if in location
 */
class	Root : public ADirective
{
	private:
		std::string	_path;

		Root();	// It needs path + context

	public:
		Root(uint16_t context, const std::string& path);
		Root(const Root& copy);
		~Root();
		Root&	operator=(const Root& other);

		ADirective*	clone() const;

		const std::string&	getPath() const;

};
