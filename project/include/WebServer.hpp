/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:38:18 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/02 18:49:33 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"
#define DEFAULT_MAX_BODY_SIZE 1048576

class	Socket;
class	Server;
class	ConfigFile;
struct	Request;

class	WebServer
{
	private:
		std::vector<ADirective*>	_configs;
		std::vector<ADirective*>	_servers;
		std::set<uint16_t>			_listenPorts;

		std::vector<Socket*>		_sockets;
		ConfigFile*					_configFile;
		Request						_clientRequest;

		void								_extractListenPorts();
		void								_closeConnection(std::string &request, std::map<std::string, std::string> &response, Socket *clientSocket);
		std::string							_readRequests(int clientSocketFD);
		void								_parseRequest(const std::string& request);
		Server*								_findVirtualServer();
		void								_matchingServersPort(std::vector<ADirective *>& servers, uint16_t requestPort);
		void								_matchingServersIp(std::vector<ADirective *>& servers, const std::string& requestIpAddress, uint16_t requestPort);
		void								_matchingServersIpPort(std::vector<ADirective *>& servers, const std::string& requestIpAddress, uint16_t requestPort);
		void								_matchingServersName(std::vector<ADirective *>& servers, const std::string& requestHost);
		std::map<std::string, std::string>	_processRequests();
		void								_sendResponse(Socket* client_socket, std::map<std::string, std::string> response);

	public:
		static WebServer *instance;

		WebServer();
		~WebServer();

		static WebServer*	getInstance();
		static void			sigintHandler(int signum);

		void	setConfigFile(ConfigFile &configFile);
		void	addConfig();
		void	addServer();

		std::vector<ADirective *>&	getConfigs();
		std::vector<ADirective *>&	getServers();
		std::vector<Socket *>&		getSockets();
		Request						getRequest();
		unsigned long long			getClientMaxBodySize();

		int		startServers();
		void	clearRequest();
};

