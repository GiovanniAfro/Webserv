/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:24:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/20 18:12:36 by gcavanna         ###   ########.fr       */
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
        std::map<string, string> _requestHeaders;
        string _requestBody;
        string _method, _uri, _httpVersion; 

        vector<uint16_t> _get_ports(void);
        string      _read_requests(Socket *client_socket);
        void        _parse_request(const string& request);
        const char *_process_requests(string request);
        void        _send_response(const char * response, Socket *client_socket);

    public:
        Http(string context);
        Http(ifstream &raw_value, string context);
        ~Http(void);

        void start_servers(void);
};
