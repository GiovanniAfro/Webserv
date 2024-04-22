/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Index.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:11 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/21 18:26:38 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_index_module.html#index
    Syntax:	    index file ...;
    Default:    index index.html;
    Context:	http, server, location
 */
class	Index : public ADirective
{
	private:
		std::vector<std::string>	_files;
		std::string					_lastElement;

		Index();

	public:
		Index(const std::string& content, uint16_t context);
		Index(const Index& copy);
		~Index();
		Index&	operator=(const Index& other);

		ADirective*	clone() const;

		std::vector<std::string>&	getFiles();
		std::string&				getLastElement();

};
