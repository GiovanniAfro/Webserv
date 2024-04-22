/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerName.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:37:49 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:37:49 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

class	ADirective;

/*!
 * @ref
		Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
		Syntax:	    server_name name ...;
		Default:	server_name "";
		Context:	server
 */

class	ServerName : public ADirective {
	private:
		std::vector<std::string>	_names;

		ServerName();

	public:
		ServerName(const std::string &content);
		ServerName(const ServerName &copy);
		~ServerName();
		ServerName &operator=(const ServerName &other);

		ADirective *clone() const;

		std::vector<std::string> &getNames();
};
