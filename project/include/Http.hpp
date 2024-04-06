/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:24:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/06 23:03:52 by adi-nata         ###   ########.fr       */
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
		vector<Socket *> 	_sockets;

		// Functionality ------------------------------------------------------>
		vector<uint16_t> 	_extract_listen_ports(void);
		string 				_read_requests(Socket *client_socket);
		void 				_parse_request(const string& request);
		int					_find_virtual_server(void);
		vector<Directive*>	_matchingServersPort(const vector<Directive*>& servers, uint16_t requestPort);
		vector<Directive*>	_matchingServersIP(const vector<Directive*>& servers, const string requestIP);
		vector<Directive*>	_matchingServersServerName(const vector<Directive*>& servers, const string requestIP);

		map<string, string> _process_requests();
		void 				_send_response(Socket *client_socket, 
										   map<string, string> response);

		// Utils -------------------------------------------------------------->
		enum HTTP_METHOD 	_methodToEnum(const string &method);
		string _statusToMessage(enum HTTP_STATUS status);
		enum HTTP_STATUS _statusToEnum(const string &status);

	public:
		static string statusToString(enum HTTP_STATUS status);
		Http(string context);
		Http(ifstream &raw_value, string context);
		~Http(void);

		void start_servers(void);
};
