/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:38:18 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 15:43:59 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

class	Socket;
class	Server;
class	ConfigFile;
struct	Request;

class	WebServer {
	private:
		std::vector<ADirective *>	_configs;
		std::vector<ADirective *>	_servers;
		std::set<uint16_t>			_listenPorts;

		std::vector<Socket *>		_sockets;
		ConfigFile *_configFile;
		Request						_clientRequest;

		void								_extractListenPorts();
		std::string							_readRequests(Socket *clientSocket);
		void								_parseRequest(const std::string &request);
		Server *_findVirtualServer();
		void								_matchingServersPort(std::vector<ADirective *> &servers, uint16_t requestPort);
		void								_matchingServersIp(std::vector<ADirective *> &servers, const std::string &requestIP, uint16_t requestPort);
		void								_matchingServersIpPort(std::vector<ADirective *> &servers, const std::string &requestIP, uint16_t requestPort);
		void								_matchingServersName(std::vector<ADirective *> &servers);
		std::map<std::string, std::string>	_processRequests();
		void								_sendResponse(Socket *client_socket, std::map<std::string, std::string> response);

	public:
		static WebServer *instance;

		WebServer();
		~WebServer();

		static WebServer *getInstance();
		static void sigintHandler(int signum);
		Http *getHttpDirective();

		void	setConfigFile(ConfigFile &configFile);
		void	addConfig();
		void	addServer();

		std::vector<ADirective *> &getConfigs();
		std::vector<ADirective *> &getServers();
		std::vector<Socket *> &getSockets();
		Request						getRequest();

		int		startServers();
		void	clearRequest();
};

