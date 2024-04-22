/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:25:22 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:33:46 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

class ADirective;

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/ngx_core_module.html#include
    Syntax:	    include file | mask;
    Default:	—
    Context:	any
 */
class Include : public ADirective {
    private:
        std::string	_path;

    public:
        Include();
        Include(const std::string &rawValue);
        Include(const Include &copy);
        ~Include();
        Include &operator=(const Include &other);

        ADirective *clone() const;

        void parse();
        const std::string	getPath() const;
};
