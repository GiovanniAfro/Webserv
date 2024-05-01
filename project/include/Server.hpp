/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:37:25 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:37:35 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

class Server : public ADirective {
	private:
		// bool		isDefault;
		// int		defaultPort;
		// string	defaultAddress;

		std::map <std::string, ADirective *>	_httpDirs;
		std::map <std::string, ADirective *>	_servDirs;
		std::map <std::string, ADirective *>	_locaDirs;
		std::string								_root;

		// HTTP methods
		std::map<std::string, std::string>	_processGet(const std::string &filePath);
		std::map<std::string, std::string>	_processPost(std::map<std::string, std::string> request, std::string const &filepath);
		std::map<std::string, std::string>	_processDelete(const std::string &filePath);


		std::string _getErrorPage(HTTP_STATUS status);
		std::map<std::string, std::string>	_responseBuilder(HTTP_STATUS status, const std::string &body = "", const std::string &contentType = "text/html");

		bool	_isMethodAllowed(const std::string &method);

		// Index and autoindex
		bool		_isAutoIndex();
		std::string	_getIndex(std::string const &filePath);
		std::map<std::string, std::string> _directoryListing(std::string const &filePath, std::string const &uri);

		std::string _getRoot();
		bool	_isBodySizeExceeded(std::map<std::string, std::string> request, std::map<std::string, std::string> requestHeaders);
		std::string _getRewrite(std::string const &requestUri);

	public:
		Server();
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &other);

		ADirective *clone() const;

		std::map<std::string, std::string>	processRequest(Http *http, Request clientRequest);
};
