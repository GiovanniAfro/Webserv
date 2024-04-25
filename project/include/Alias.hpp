/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Alias.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:32 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 21:44:56 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Libraries ------------------------------------------------------------------>

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#alias
    Syntax:	    alias path;
    Default:	—
    Context:	location
 */
class Alias : public ADirective {
	private:
		std::string	_path;
		
		Alias();

	public:
		Alias(const std::string& path);
		Alias(const Alias& copy);
		~Alias();
		Alias&	operator=(const Alias& other);
		ADirective*	clone() const;
		const std::string&	getPath() const;
};
