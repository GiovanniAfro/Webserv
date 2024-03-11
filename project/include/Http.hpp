/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:24:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/11 12:28:07 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"
#include "Socket.hpp"

class Socket;

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#http
    Syntax:	    http { ... }
    Default:	—
    Context:	main
 */
class Http : public Directive {
    private:
        // void _signal_handler(int signal);
        vector<int> _get_ports(void);
        string _read_requests(Socket *client_socket);
        const char *_process_requests(string request);
        void _send_response(const char * response, Socket *client_socket);

    public:
        Http(string context);
        Http(ifstream &raw_value, string context);
        ~Http();

        void start_servers();
};
