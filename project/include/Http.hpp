/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:24:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/04 15:46:12 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"
#include "Socket.hpp"

class Socket;

enum HTTP_METHOD {
	GET,
	POST,
	DELETE,
	UNKNOWN
};

enum HTTP_STATUS {
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500
};

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
		map<string, string> _request;
		map<string, string> _requestHeaders;
		string 				_requestBody;
		enum HTTP_METHOD 	_requestMethod;
		enum HTTP_STATUS 	_responseStatus;

		// Functionality ------------------------------------------------------>
		vector<uint16_t> 	_extract_listen_ports(void);
		string 				_read_requests(Socket *client_socket);
		void 				_parse_request(const string& request);
		int					_find_virtual_server(void);
		map<string, string> _process_requests();
		void 				_send_response(Socket *client_socket);

		// Utils -------------------------------------------------------------->
		enum HTTP_METHOD 	_methodToEnum(const string &method);
		string 				_statusToString(enum HTTP_STATUS status);

	public:
		Http(string context);
		Http(ifstream &raw_value, string context);
		~Http(void);

		void start_servers(void);
};
