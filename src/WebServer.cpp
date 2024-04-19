#include "WebServer.hpp"

WebServer::WebServer() {}

// WebServer::WebServer() {}

WebServer::~WebServer()
{
	Log::debug("WebServer Desc");
	for (std::vector<ADirective*>::iterator it = _configs.begin(); it != _configs.end(); ++it)
		delete (*it);
	for (std::vector<ADirective*>::iterator it = _servers.begin(); it != _servers.end(); ++it)
		delete (*it);

}

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
	Log::debug("addServer");
	_servers.push_back(new Server());
	// if (!_servers.empty())
	// {
	// 	_servers.back()->addBlock(_servers.back());
	// }
}

std::vector<ADirective*>&	WebServer::getConfigs()
{ return _configs; }

std::vector<ADirective*>&	WebServer::getServers()
{ return _servers; }

std::vector<Socket*>&	WebServer::getSockets()
{ return _sockets; }

Request	WebServer::getRequest()
{ return _clientRequest; }

int	WebServer::startServers()
{
	std::set<uint16_t>					ports = this->_extractListenPorts();
	struct pollfd						fds[ports.size()];
	unsigned int						numFds = 0;
	Socket*								clientSocket;
	std::string							request;
	std::map<std::string, std::string>	response;

	// Creates sockets and adds them to the pollfd structure ------------------>
	for (std::set<uint16_t>::iterator it = ports.begin(); it != ports.end(); ++it)
	{
		std::cout << *it << std::endl;
		Socket*	newSocket = new Socket(*it);
		this->_sockets.push_back(newSocket);
		fds[numFds].fd = newSocket->getSocket();
		fds[numFds].events = POLLIN | POLLOUT;
		++numFds;
	}

	// Polling ---------------------------------------------------------------->
	while (true)
	{
		int	pollStatus = poll(fds, numFds, -1);
		if (pollStatus == -1)
		{
			Log::error("Poll failed");
			return -1;
		}

		// Handle events on sockets ------------------------------------------->
		for (unsigned int i = 0; i < numFds; ++i)
		{
			if (!(fds[i].revents & POLLIN))
				continue;

			// Accept connection and create new socket for client---------->
			clientSocket = this->_sockets[i]->createClientSocket();

			// Read the request ------------------------------------------->
			request = this->_readRequests(clientSocket);
			if (request.empty())
			{
				Log::error("Request reading failed");
				// return -1;
			}

			// Parse the request -------------------------------------->
			this->_parseRequest(request);

			// Process the requests ----------------------------------->
			response = this->_processRequests();

			// Send the response -------------------------------------->
			this->_sendResponse(clientSocket, response);

			// Empty the request and response -------------------------->
			request.clear();
			response.clear();

		}

		// Close the client socket ------------------------------------>
		clientSocket->closeSocket();
		delete clientSocket;
	}

	return 0;
}

void	WebServer::clearRequest()
{
	this->_clientRequest.request.clear();
	this->_clientRequest.requestHeaders.clear();
	this->_clientRequest.requestBody.clear();
	this->_clientRequest.requestMethod = UNKNOWN;
}

std::set<uint16_t>	WebServer::_extractListenPorts()
{
	std::set<uint16_t>	ports;

	for (std::vector<ADirective*>::iterator itServer = this->getServers().begin(); itServer != this->getServers().end(); ++itServer)
	{
		std::vector<ADirective*>	listenBlocks = (*itServer)->getDirectives()["listen"]->getBlocks();
		for (std::vector<ADirective*>::iterator itListen = listenBlocks.begin(); itListen != listenBlocks.end(); ++itListen)
		{
			Listen*	listen = static_cast<Listen*>(*itListen);
			if (listen->getPorts().size() == 0)
				continue;
			for (std::set<uint16_t>::iterator itPorts = listen->getPorts().begin(); itPorts != listen->getPorts().end(); ++itPorts)
				ports.insert(*itPorts);
		}
	}

	return ports;
}

std::string	WebServer::_readRequests(Socket* clientSocket)
{
	char				buf[4096]; // Buffer più grande per gestire la maggior parte delle richieste in un solo ciclo
	std::string			request = "";
	ssize_t				bytes_read;
	const std::string	request_end = "\r\n\r\n";
	size_t				found_end;

	while (true)
	{
		bytes_read = recv(clientSocket->getSocket(), buf, sizeof(buf) - 1, 0); //La funzione recv può essere utilizzata anche per ricevere dati in modo non bloccante, utilizzando la flag MSG_DONTWAIT. Inoltre, la funzione recv può essere utilizzata per ricevere dati da più socket contemporaneamente, utilizzando la funzione select.

		if (bytes_read > 0)
		{
			buf[bytes_read] = '\0'; // Assicurati che la std::stringa sia terminata correttamente
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
					if (content_length_end != std::string::npos)
					{
						Log::error("Richiesta malformata: impossibile trovare la fine dell'header Content-Lenght");
						return "";
					}
					int content_length = atoi(request.substr(content_length_pos, content_length_end - content_length_pos).c_str());
					size_t headers_length = found_end + 4; // +4 per la sequenza \r\n\r\n

					// Leggi il corpo della richiesta se non è stato già completamente ricevuto
					while (request.size() < headers_length + static_cast<size_t>(content_length))
					{
						bytes_read = recv(clientSocket->getSocket(), buf, sizeof(buf) - 1, 0);
						if (bytes_read <= 0) break; // Gestisci errori o chiusura connessione
						buf[bytes_read] = '\0';
						request.append(buf);
					}
				}
				break; // Hai trovato la fine dell'header e letto il corpo se necessario
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
	// std::cout << request << std::endl;
	return request;
}

void	WebServer::_parseRequest(const std::string& request)
{
	Http*				http = static_cast<Http*>(this->getConfigs()[0]);
	std::stringstream	requestStream(request);
	std::string			line;
	getline(requestStream, line);
	std::stringstream	firstLineStream(line);

	// Extract the request method, URI and HTTP version
	firstLineStream >> this->_clientRequest.request["method"] >> this->_clientRequest.request["uri"] >> this->_clientRequest.request["httpVersion"];
	this->_clientRequest.requestMethod = http->_methodToEnum(this->_clientRequest.request["method"]);

	// Extract the request headers
	while (getline(requestStream, line) && !line.empty()) {
		std::string::size_type colonPos = line.find(": ");
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerValue = line.substr(colonPos + 2);
			this->_clientRequest.requestHeaders[headerName] = headerValue;
			std::cout << headerName << " -> " << headerValue << std::endl;
		}
	}

	// Extract the request body specified by the Content-Length header
	std::map<std::string, std::string>::iterator it = this->_clientRequest.requestHeaders.find("Content-Length");
	if (it != this->_clientRequest.requestHeaders.end())
	{
		int contentLength = atoi(it->second.c_str());
		this->_clientRequest.requestBody.reserve(static_cast<std::string::size_type>(contentLength));
		while (contentLength > 0 && getline(requestStream, line))
		{
			this->_clientRequest.requestBody.append(line + "\n");
			contentLength -= line.length() + 1; // +1 per il carattere di nuova linea che getline consuma
		}
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
	Server*	server = _findVirtualServer();

	return server->processRequest(this->_clientRequest.request);
}

Server*	WebServer::_findVirtualServer()
{
	std::string					requestHost = this->_clientRequest.requestHeaders["Host"];
	std::string					requestIP = requestHost.substr(0, requestHost.find(":")).c_str();
	uint16_t					requestPort = static_cast<uint16_t>(atoi(requestHost.substr(requestHost.find(":") + 1).c_str()));
	std::vector<ADirective *>	matchingServers = this->getServers();
	Server*						virtualServer = NULL;
	std::stringstream			stream;

	// Log::debug("_findVirtualServer");
	// std::cout << "requestHost : " << requestHost << std::endl;

	this->_matchingServersPort(matchingServers, requestPort);

	this->_matchingServersIp(matchingServers, requestIP);

	// this->_matchingServersIpPort(matchingServers, requestIP, requestPort);

	// this->_matchingServersName(matchingServers);

	if (matchingServers.size() == 0)
	{
		Log::error("Matching virtual server not found");
	}
	else //if (matchingServers.size() > 1)
		virtualServer = static_cast<Server*>(matchingServers[0]);

	return virtualServer;
}

void	WebServer::_matchingServersPort(std::vector<ADirective *>& servers, uint16_t requestPort)
{
	for (std::vector<ADirective*>::iterator itServer = servers.begin(); itServer != servers.end(); ++itServer)
	{
		ADirective*	listenDiretives = (*itServer)->getDirectives()["listen"];
		bool		isMatch = false;

		for (std::vector<ADirective*>::iterator itListen = listenDiretives->getBlocks().begin(); itListen != listenDiretives->getBlocks().end(); ++itListen)
		{
			Listen*	listen = static_cast<Listen*>(*itListen);


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

void	WebServer::_matchingServersIp(std::vector<ADirective *>& servers, const std::string& requestIP)
{
	for (std::vector<ADirective*>::iterator itServer = servers.begin(); itServer != servers.end(); )
	{
		ADirective*	listenDiretives = (*itServer)->getDirectives()["listen"];
		bool		isMatch = false;

		for (std::vector<ADirective*>::iterator itListen = listenDiretives->getBlocks().begin(); itListen != listenDiretives->getBlocks().end(); ++itListen)
		{
			Listen*	listen = static_cast<Listen*>(*itListen);

			for (std::vector<std::string>::iterator itIP = listen->getAddress().begin(); itIP != listen->getAddress().end(); ++itIP)
			{

				if ((*itIP) == requestIP)
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
		else
			++itServer;
	}
}

void	WebServer::_matchingServersIpPort(std::vector<ADirective *>& servers, const std::string& requestIP, uint16_t requestPort)
{
	for (std::vector<ADirective*>::iterator itServer = servers.begin(); itServer != servers.end(); )
	{
		ADirective*	listenDiretives = (*itServer)->getDirectives()["listen"];
		bool		isMatch = false;

		for (std::vector<ADirective*>::iterator itListen = listenDiretives->getBlocks().begin(); itListen != listenDiretives->getBlocks().end(); ++itListen)
		{
			Listen*	listen = static_cast<Listen*>(*itListen);

			if (listen->getAddressPort().find(requestIP) != listen->getAddressPort().end())
			{
				for (std::set<uint16_t>::iterator itPort = listen->getAddressPort()[requestIP].begin(); itPort != listen->getAddressPort()[requestIP].end(); ++itPort)
				{
					if ((*itPort) == requestPort)
					{
						// Matching address:port
						isMatch = true;
						break;
					}
				}
			}
			if (isMatch == true)
				break;
		}
		if (isMatch == false)
			itServer = servers.erase(itServer);
		else
			++itServer;
	}

}

void	WebServer::_matchingServersName(std::vector<ADirective *>& servers)
{
	for (std::vector<ADirective*>::iterator itServer = servers.begin(); itServer != servers.end(); )
	{
		ADirective*	listenDiretives = (*itServer)->getDirectives()["listen"];
		bool		isMatch = false;

		for (std::vector<ADirective*>::iterator itListen = listenDiretives->getBlocks().begin(); itListen != listenDiretives->getBlocks().end(); ++itListen)
		{
			// Listen*	listen = static_cast<Listen*>(*itListen);



			if (isMatch == true)
				break;
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

	if (bytes_sent == -1) {
		Log::error("Sending the response failed");
		client_socket->closeSocket();
	}
}