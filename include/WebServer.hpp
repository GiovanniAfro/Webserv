#pragma once

#include "webserv.h"

class	Socket;
class	Server;
struct	Request;

class	WebServer
{
	private:
		std::vector<ADirective*>	_configs;
		std::vector<ADirective*>	_servers;

		std::vector<Socket*>		_sockets;
		Request						_clientRequest;

		std::set<uint16_t>					_extractListenPorts();
		std::string							_readRequests(Socket* clientSocket);
		void								_parseRequest(const std::string& request);
		Server*								_findVirtualServer();
		void								_matchingServersPort(std::vector<ADirective *>& servers, uint16_t requestPort);
		void								_matchingServersIp(std::vector<ADirective *>& servers, const std::string& requestIP);
		void								_matchingServersIpPort(std::vector<ADirective *>& servers, const std::string& requestIP, uint16_t requestPort);
		void								_matchingServersName(std::vector<ADirective *>& servers);
		std::map<std::string, std::string>	_processRequests();
		void								_sendResponse(Socket *client_socket, std::map<std::string, std::string> response);


	public:
		WebServer();
		// WebServer();
		~WebServer();

		void	addConfig();
		void	addServer();

		std::vector<ADirective*>&	getConfigs();
		std::vector<ADirective*>&	getServers();
		std::vector<Socket*>&		getSockets();
		Request						getRequest();


		int		startServers();
		void	clearRequest();
};

