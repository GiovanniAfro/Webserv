/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:25:08 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/04 16:33:11 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

/*!
 * @ref
    Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#server
    Syntax:	    server { ... }
    Default:	—
    Context:	http
 */
class Server : public Directive {
    public:
        Server(string context);
        Server(ifstream &raw_value, string context);
        ~Server();

        map<string, string> process_request(map<string, string> request);

        vector<Listen *>            get_listen(void);
        vector<ServerName *>        get_server_name(void);
        vector<Root *>              get_root(void);
        vector<Alias *>             get_alias(void);
        vector<Autoindex *>         get_autoindex(void);
        vector<ErrorPage *>         get_error_page(void);
        vector<Rewrite *>           get_rewrite(void);
        vector<ClientMaxBodySize *> get_client_max_body_size(void);
        vector<Index *>             get_index(void);
        // get_location();
        // get_limit_except();
};
