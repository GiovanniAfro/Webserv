/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:45:25 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/09 11:47:06 by kichkiro         ###   ########.fr       */
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
class LimitExcept : public Directive {
    public:
        LimitExcept(string context);
        LimitExcept(string line, ifstream &block, string context);
        ~LimitExcept();
};
