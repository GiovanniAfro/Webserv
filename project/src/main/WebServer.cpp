/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:49:22 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/06 15:02:20 by gcavanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer *WebServer::instance = NULL;

WebServer::WebServer() : _shutdownFlag(false)
{
	instance = this;
	signal(SIGINT, sigintHandler);
}

WebServer::~WebServer()
{
	for (std::vector<ADirective *>::iterator it = _configs.begin(); it != _configs.end(); ++it)
		delete (*it);
	for (std::vector<ADirective *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
		delete (*it);
	for (std::vector<Socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
		delete (*it);
	for (std::set<uint16_t>::iterator it = _listenPorts.begin(); it != _listenPorts.end(); ++it)
		close(*it);
}

void WebServer::sigintHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Shutdown signal received, shutting down..." << std::endl;
		if (instance)
        	WebServer::instance->_shutdownFlag = true;  // Imposta una flag per fermare il loop del server.
    }
}


void	WebServer::setConfigFile(ConfigFile &configFile)
{ _configFile = &configFile; }

void	WebServer::addConfig()
{
	_configs.push_back(new Http());
	// if (!_configs.empty())
	// {
	// 	_configs.back()->addBlock(_configs.back());
	// }
}

void	WebServer::addServer()
{
	// // Log::debug("addServer");
	_servers.push_back(new Server());
	// if (!_servers.empty())
	// {
	// 	_servers.back()->addBlock(_servers.back());
	// }
}

std::vector<ADirective *> &WebServer::getConfigs()
{ return _configs; }

std::vector<ADirective *> &WebServer::getServers()
{ return _servers; }

std::vector<Socket *> &WebServer::getSockets()
{ return _sockets; }

Request	WebServer::getRequest()
{ return _clientRequest; }

unsigned long long	WebServer::getClientMaxBodySize()
{
	if (this->getConfigs()[0]->getDirectives().find("client_max_body_size") == this->getConfigs()[0]->getDirectives().end())
		return CLIENT_MAX_BODY_SIZE;

	return static_cast<ClientMaxBodySize*>(this->getConfigs()[0]->getDirectives()["client_max_body_size"])->getSize();
}

int	WebServer::startServers()
{
	this->_extractListenPorts();
	struct pollfd						fds[_listenPorts.size()];
	unsigned int						numFds = 0;
	Socket*								clientSocket;
	std::string							request;
	std::map<std::string, std::string>	response;

	// Creates sockets and adds them to the pollfd structure ------------------>
	for (std::set<uint16_t>::iterator it = _listenPorts.begin(); it != _listenPorts.end(); ++it)
	{
		Socket *newSocket = new Socket(*it);
		this->_sockets.push_back(newSocket);
		fds[numFds].fd = newSocket->getSocket();
		fds[numFds].events = POLLIN | POLLOUT;
		++numFds;
	}

	// Polling ---------------------------------------------------------------->
	while (!_shutdownFlag)
	{
		// When ctrl-c signal : here
		int	pollStatus = poll(fds, numFds, -1);
		if (pollStatus == -1) {
    		if (errno == EINTR) {
        		// Gestione dell'interruzione (es. pulizia e chiusura)
        		Log::info("Poll interrupted by signal, shutting down gracefully.");
        		return 0; // o un altro codice che gestisce la chiusura ordinata
    		} else {
        		// Log dell'errore effettivo
        		Log::error("Poll failed due to system error: ");
        		return -1; // Indica un errore serio che non può essere ignorato
    		}
		}
		// Handle events on sockets ------------------------------------------->
		for (unsigned int i = 0; i < numFds; ++i)
		{
			if (!(fds[i].revents & POLLIN))
				continue;

			// Accept connection and create new socket for client---------->
			clientSocket = this->_sockets[i]->createClientSocket();
			if (!clientSocket)
				return Log::error("Client socket creation failed");

				
			// Read the request ------------------------------------------->
			request = this->_readRequests(clientSocket->getSocket());
			if (request.empty() || request == "request")
				Log::error("Request reading failed");
			else
			{
				// Parse the request -------------------------------------->
				this->_parseRequest(request);

				// Process the requests ----------------------------------->
				response = this->_processRequests();
				if (response.empty())
				{
					Log::error("WebServer : empty response");
					_closeConnection(request, response, clientSocket);
					continue;
				}

				// Send the response -------------------------------------->
				this->_sendResponse(clientSocket, response);

				// If the response is a redirection, close the client socket
				// then create a new socket and send the redirection response
				if (response["status"] == "302")
				{
					_clientRequest.request["uri"] = response["Location"];
					response = this->_processRequests();
					if (response.empty())
					{
						Log::error("WebServer : empty response");
						_closeConnection(request, response, clientSocket);
						continue;
					}
					this->_sendResponse(clientSocket, response);
				}
			}
			_closeConnection(request, response, clientSocket);
		}
	}

	return 0;
}

void	WebServer::_closeConnection(std::string &request, std::map<std::string, std::string> &response, Socket *clientSocket)
{
	response.clear();
	request.clear();
	clientSocket->closeSocket();
	delete clientSocket;
	clientSocket = NULL;
}

void	WebServer::clearRequest()
{
	this->_clientRequest.request.clear();
	this->_clientRequest.requestHeaders.clear();
	this->_clientRequest.requestBody.clear();
	this->_clientRequest.requestMethod = UNKNOWN;
}

void	WebServer::_extractListenPorts()
{
	for (std::vector<ADirective *>::iterator itServer = this->getServers().begin(); itServer != this->getServers().end(); ++itServer)
	{
		std::vector<ADirective *>	listenBlocks = (*itServer)->getDirectives()["listen"]->getBlocks();
		for (std::vector<ADirective *>::iterator itListen = listenBlocks.begin(); itListen != listenBlocks.end(); ++itListen)
		{
			Listen *listen = static_cast<Listen *>(*itListen);
			for (std::set<uint16_t>::iterator itPorts = listen->getPorts().begin(); itPorts != listen->getPorts().end(); ++itPorts)
				_listenPorts.insert(*itPorts);
		}
	}
}

std::string	WebServer::_readRequests(int clientSocketFD)
{
	char				buf[this->getClientMaxBodySize()]; // Buffer più grande per gestire la maggior parte delle richieste in un solo ciclo
	std::string			request;
	ssize_t				bytes_read;
	const std::string	request_end = "\r\n\r\n";
	size_t				found_end;

	while (true)
	{
		// La funzione recv può essere utilizzata anche per ricevere dati in modo non bloccante, utilizzando la flag MSG_DONTWAIT. Inoltre, la funzione recv può essere utilizzata per ricevere dati da più socket contemporaneamente, utilizzando la funzione select.
		bytes_read = recv(clientSocketFD, buf, sizeof(buf) - 1, 0);

		if (bytes_read > 0)
		{
			// Assicurati che la std::stringa sia terminata correttamente
			buf[bytes_read] = '\0';
			request.append(buf);

			// Controlla se hai ricevuto l'intero header della richiesta
			found_end = request.find(request_end);
			if (found_end != std::string::npos)
			{
				// Se esiste un "Content-Length", devi leggere anche il corpo della richiesta
				size_t content_length_pos = request.find("Content-Length: ");
				if (content_length_pos != std::string::npos)
				{
					content_length_pos += strlen("Content-Length: ");
					size_t content_length_end = request.find("\r\n", content_length_pos);
					if (content_length_end == std::string::npos)
					{
						Log::error("Richiesta malformata: impossibile trovare la fine dell'header Content-Lenght");
						return "";
					}
					int content_length = atoi(request.substr(content_length_pos, content_length_end - content_length_pos).c_str());
					size_t headers_length = found_end + 4; // +4 per la sequenza \r\n\r\n

					// Leggi il corpo della richiesta se non è stato già completamente ricevuto
					while (request.size() < headers_length + static_cast<size_t>(content_length))
					{
						bytes_read = recv(clientSocketFD, buf, sizeof(buf) - 1, 0);
						// Gestisci errori o chiusura connessione
						if (bytes_read <= 0)
							break;
						buf[bytes_read] = '\0';
						request += buf;
					}
				}
				// Hai trovato la fine dell'header e letto il corpo se necessario
				break;
			}
		}
		else if (bytes_read == 0)
		{
			// La connessione è stata chiusa dal client
			Log::info("Connection closed by remote host");
			return "";
		}
		else
		{
			// Gestisci errori di lettura
			Log::error("Error reading from socket");
			return "";
		}
	}
	if (found_end == std::string::npos)
	{
		Log::error("Richiesta malformata: impossibile trovare la fine dell'header Content-Lenght");
		return "";
	}
	std::cout << request << std::endl;
	return request;
	// std::string r2 = request.substr(0, found_end + 4);
	// return r2;
}

void WebServer::_parseRequest(const std::string &request) {
    std::istringstream requestStream(request);
    std::string line;
    std::getline(requestStream, line);
    std::istringstream firstLineStream(line);
    int contentLength = 0;

    // Extract the request method, URI, and HTTP version
    firstLineStream >> this->_clientRequest.request["method"]
                    >> this->_clientRequest.request["uri"]
                    >> this->_clientRequest.request["httpVersion"];
    this->_clientRequest.requestMethod = Http::_methodToEnum(this->_clientRequest.request["method"]);

    // Extract the request headers
    std::string headers;
    while (std::getline(requestStream, line) && !line.empty() && line != "\r") {
        std::size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos) {
            std::string headerName = line.substr(0, colonPos);
            std::string headerValue = strip(line.substr(colonPos + 2));

            this->_clientRequest.requestHeaders[headerName] = headerValue;

            if (headerName == "Content-Type" && headerValue.find("boundary=") != std::string::npos) {
                size_t boundaryPos = headerValue.find("boundary=") + 9; // 9 is the length of "boundary="
                this->_clientRequest.requestHeaders["boundary"] = headerValue.substr(boundaryPos);
            }

            if (headerName == "Content-Length") {
                contentLength = std::atoi(headerValue.c_str());
            }
        }
    }

    // Extract the request body specified by the Content-Length header
    if (contentLength > 0) {
        this->_clientRequest.requestBody.reserve(contentLength);
        char buffer[contentLength + 1]; // +1 to ensure null termination
        requestStream.read(buffer, contentLength);
        buffer[contentLength] = '\0'; // Ensure null termination
        this->_clientRequest.requestBody.assign(buffer, contentLength);
    }
}


/*!
 * @brief
	- Trova il blocco server a cui e' indirizzata la richiesta,
	- Instrada la richiesta al blocco server corretto.
	- Il blocco server elabora la richiesta e ritorna la risposta.
 */
std::map<std::string, std::string>	WebServer::_processRequests()
{
	Server *server = _findVirtualServer();

	if (!server)
	{
		Log::error("WebServer : _processRequests : matching server not found");
		return std::map<std::string, std::string>();
	}

	// for (std::map<std::string, ADirective*>::iterator it = server->getDirectives().begin();it != server->getDirectives().end(); ++it)
	// {
	// 	Log::debug((*it).first);
	// }

	return server->processRequest(static_cast<Http *>(this->getConfigs()[0]), this->_clientRequest);
}

Server*	WebServer::_findVirtualServer()
{
	std::string					requestHost = this->_clientRequest.requestHeaders["Host"];
	std::string					requestAddress = requestHost;
	uint16_t					requestPort = 80;
	std::vector<ADirective *>	matchingServers = this->getServers();
	Server*						virtualServer = NULL;
	std::stringstream			stream;

	if (requestHost.find(":") != std::string::npos)
	{
		requestAddress = requestHost.substr(0, requestHost.find(":")).c_str();
		requestPort = static_cast<uint16_t>(atoi(requestHost.substr(requestHost.find(":") + 1).c_str()));
	}

	this->_matchingServersPort(matchingServers, requestPort);

	if (isIPAddress(requestAddress))
	{
		this->_matchingServersIp(matchingServers, requestAddress, requestPort);
		if (matchingServers.size() > 1)
			this->_matchingServersName(matchingServers, requestAddress);
	}
	else
		this->_matchingServersName(matchingServers, requestAddress);
		
	if (matchingServers.size() == 0)
	{
		// First default virtual server? -> default Server* in WebServer

		Log::error("Matching virtual server not found");
	}
	else //if (matchingServers.size() > 1)
		virtualServer = static_cast<Server *>(matchingServers[0]);

	return virtualServer;
}

void	WebServer::_matchingServersPort(std::vector<ADirective *> &servers, uint16_t requestPort)
{
	for (std::vector<ADirective *>::iterator itServer = servers.begin(); itServer != servers.end(); ++itServer)
	{
		ADirective *listenDiretives = (*itServer)->getDirectives()["listen"];
		bool		isMatch = false;

		for (std::vector<ADirective *>::iterator itListen = listenDiretives->getBlocks().begin(); itListen != listenDiretives->getBlocks().end(); ++itListen)
		{
			Listen *listen = static_cast<Listen *>(*itListen);

			for (std::set<uint16_t>::iterator itPort = listen->getPorts().begin(); itPort != listen->getPorts().end(); ++itPort)
			{
				if ((*itPort) == requestPort)
				{
					isMatch = true;
					break;
				}
			}
			if (isMatch == true)
				break;
		}
		if (isMatch == false)
			itServer = servers.erase(itServer);
	}
}

void	WebServer::_matchingServersIp(std::vector<ADirective *> &servers, const std::string &requestIpAddress, uint16_t requestPort)
{
	for (std::vector<ADirective *>::iterator itServer = servers.begin(); itServer != servers.end(); )
	{
		ADirective *listenDiretives = (*itServer)->getDirectives()["listen"];
		bool		isMatch = false;

		for (std::vector<ADirective *>::iterator itListen = listenDiretives->getBlocks().begin(); itListen != listenDiretives->getBlocks().end(); ++itListen)
		{
			Listen *listen = static_cast<Listen *>(*itListen);

			if (listen->getAddress().empty())
				continue;

			if (listen->getAddress() == requestIpAddress &&
				listen->getPorts().find(requestPort) != listen->getPorts().end())
			{
				isMatch = true;
				break;
			}
		}
		if (isMatch == false)
			itServer = servers.erase(itServer);
		else
			++itServer;
	}
}

void	WebServer::_matchingServersName(std::vector<ADirective *>& servers, const std::string& requestHost)
{
	for (std::vector<ADirective *>::iterator itServer = servers.begin(); itServer != servers.end(); )
	{
		ServerName*	serverName = static_cast<ServerName*>((*itServer)->getDirectives()["server_name"]);
		bool		isMatch = false;

		for (std::vector<std::string>::iterator it = serverName->getNames().begin(); it != serverName->getNames().end(); ++it)
		{
			if (*it == requestHost)
			{
				isMatch = true;
				break;
			}
		}
		if (isMatch == false)
			itServer = servers.erase(itServer);
		else
			++itServer;
	}
}

void	WebServer::_sendResponse(Socket *client_socket, std::map<std::string, std::string> response)
{
	HTTP_STATUS status = Http::_statusToEnum(response["status"]);

	Log::response(this->_clientRequest.requestMethod, this->_clientRequest.request["httpVersion"],
				  this->_clientRequest.request["uri"], status);

	std::string	s_response = (this->_clientRequest.request["httpVersion"] + " " +
					   Http::_statusToMessage(status) +
					   "\r\nContent-Type: " + response["Content-Type"] +
					   "\r\n\r\n" + response["body"]);

	ssize_t bytes_sent = send(client_socket->getSocket(), s_response.c_str(),
							  strlen(s_response.c_str()), 0);

	// TODO:
	// Se non si riesce ad inviare tutti i dati, impostare POLLOUT ed inviare
	// i dati rimanenti appena e' possibile scrivere.

	if (bytes_sent == -1)
	{
		Log::error("Sending the response failed");
		client_socket->closeSocket();
	}
}
