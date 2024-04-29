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

		// HTTP methods
		std::map<std::string, std::string>	_processGet(const std::string &filePath);
		std::map<std::string, std::string>	_processPost(std::map<std::string, std::string> request, std::string const &filepath);
		std::map<std::string, std::string>	_processDelete(const std::string &filePath);


		std::string _getErrorPage(HTTP_STATUS status);
		std::map<std::string, std::string>	_responseBuilder(HTTP_STATUS status, const std::string &body = "", const std::string &contentType = "text/html");

		bool	_isMethodAllowed(const std::string &method, std::map<std::string, ADirective *> locaDirs);

		// Index and autoindex
		bool	_isFolder(const std::string &path);
		bool	_isFile(const std::string &path);
		bool		_isAutoIndex(std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs);
		std::string	_getIndex(const std::string &path, std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs);
		std::map<std::string, std::string> _directoryListing(std::string const &path, std::string const &uri);

		std::string _getRoot(std::map<std::string, ADirective *> httpDirs, std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs);
		bool	_isBodySizeExceeded(std::map<std::string, std::string> request, std::map<std::string, std::string> requestHeaders, std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs);

	public:
		Server();
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &other);

		ADirective *clone() const;

		std::map<std::string, std::string>	processRequest(Http *http, std::map<std::string, std::string> request, std::map<std::string, std::string> requestHeaders);
};
