/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:41:07 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/08 15:21:23 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
    Syntax:	    listen address[:port];
    Default:	listen *:80;
    Context:	server
 */
class Listen : public Directive {
    public:
        Listen(string context);
        Listen(string raw_value, string context);
        ~Listen();

        void _validate_config(void);

        class WrongNumArgsExc : public exception {
            private:
                virtual const char *what(void) const throw() {return "err";}
            public:
                WrongNumArgsExc(void) {
                    cerr << "webserv: Listen: wrong number of args" << endl;
                }
                virtual ~WrongNumArgsExc() throw() {}
        };

        class WrongValueArgExc : public exception {
            private:
                virtual const char *what(void) const throw() {return "err";}
            public:
                WrongValueArgExc(string arg) {
                    cerr << "webserv: Listen: wrong arg: " << arg << endl;
                }
                virtual ~WrongValueArgExc() throw() {}
        };
};
