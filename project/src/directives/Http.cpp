/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:47:13 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/29 17:41:08 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(string context) {
	vector<Directive *> value;

	if (context != "main")
		throw WrongContextExc("Http", "main", context);
	this->_type = "http";
	this->_is_context = true;
	this->_value_block = value;
}

Http::Http(ifstream &raw_value, string context) {
	if (context != "main")
		throw WrongContextExc("Http", "main", context);
	this->_type = "http";
	this->_is_context = true;
	this->_parsing_block(raw_value);
}

Http::~Http(void) {
	for (VecDirIt it = this->_value_block.begin();
		it != this->_value_block.end(); ++it)
		delete *it;
}

vector<uint16_t> Http::_get_ports(void) {
	vector<uint16_t> ports;
	size_t num_servers = this->_value_block.size();
	size_t num_directives;
	size_t num_ports;
	uint16_t port;

	for (size_t i = 0; i < num_servers; i++) {
		num_directives = this->_value_block[i]->get_block_size();
		for (size_t j = 0; j < num_directives; j++) {
			if (this->_value_block[i]->get_value_block()[j]->get_type()
				== "listen") {
				num_ports = this->_value_block[i]->get_value_block()[j]
					->get_inline_size();
				for (size_t k = 0; k < num_ports; k++) {
					if (this->_value_block[i]->get_value_block()[j]
						->get_value_inline()[k] != "default_server") {
						port = static_cast<uint16_t>(atoi(
							this->_value_block[i]->get_value_block()[j]
							->get_value_inline()[k].c_str()));
						if (!uint16_t_in_vec(ports, port))
							ports.push_back(port);
					}
				}
			}
		}
	}
	return ports;
}

// DA SISTEMARE
string Http::_read_requests(Socket *client_socket)
{
	char buf[4096]; // Buffer più grande per gestire la maggior parte delle richieste in un solo ciclo
	string request = "";
	ssize_t bytes_read;
	const string request_end = "\r\n\r\n";
	size_t found_end;

	while (true)
	{
		bytes_read = recv(client_socket->get_socket(), buf, sizeof(buf) - 1, 0); //La funzione recv può essere utilizzata anche per ricevere dati in modo non bloccante, utilizzando la flag MSG_DONTWAIT. Inoltre, la funzione recv può essere utilizzata per ricevere dati da più socket contemporaneamente, utilizzando la funzione select.

		if (bytes_read > 0)
		{
			buf[bytes_read] = '\0'; // Assicurati che la stringa sia terminata correttamente
			request.append(buf);

			// Controlla se hai ricevuto l'intero header della richiesta
			found_end = request.find(request_end);
			if (found_end != string::npos)
			{
				// Se esiste un "Content-Length", devi leggere anche il corpo della richiesta
				size_t content_length_pos = request.find("Content-Length: ");
				if (content_length_pos != string::npos)
				{
					content_length_pos += strlen("Content-Length: ");
					size_t content_length_end = request.find("\r\n", content_length_pos);
					if (content_length_end != string::npos)
					{
						Log::error("Richiesta malformata: impossibile trovare la fine dell'header Content-Lenght");
						return "";
					}
					int content_length = atoi(request.substr(content_length_pos, content_length_end - content_length_pos).c_str());
					size_t headers_length = found_end + 4; // +4 per la sequenza \r\n\r\n

					// Leggi il corpo della richiesta se non è stato già completamente ricevuto
					while (request.size() < headers_length + content_length)
					{
						bytes_read = recv(client_socket->get_socket(), buf, sizeof(buf) - 1, 0);
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
	if (found_end == string::npos)
	{
		Log::error("Richiesta malformata: impossibile trovare la fine dell'header Content-Lenght");
		return "";
	}
    cout << request << endl;
	return request;
}

void Http::_parse_request(const string& request)
{
	std::istringstream requestStream(request);
	std::string line;
	getline(requestStream, line);
	std::istringstream firstLineStream(line);

	// Extract the request method, URI and HTTP version
	firstLineStream >> _request["method"] >> _request["uri"] >> _request["httpVersion"];
	_requestMethod = _methodToEnum(_request["method"]);

	// Extract the request headers
	while (getline(requestStream, line) && !line.empty()) {
		std::string::size_type colonPos = line.find(": ");
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerValue = line.substr(colonPos + 2);
			_requestHeaders[headerName] = headerValue;
			cout << _requestHeaders[headerName] << endl;
		}
	}

	// Extract the request body specified by the Content-Length header
	std::map<std::string, std::string>::iterator it = _requestHeaders.find("Content-Length");
	if (it != _requestHeaders.end()) {
		int contentLength = atoi(it->second.c_str());
		_requestBody.reserve(contentLength);
		while (contentLength > 0 && getline(requestStream, line)) {
			_requestBody.append(line + "\n");
			contentLength -= line.length() + 1; // +1 per il carattere di nuova linea che getline consuma
		}
	}
}

void Http::_process_requests()
{
	// Individuare il virtual server corretto ed inviargli la richiesta,
	// quest'ultimo provvedera' a restituire la risposta
	// TMP
	// _responseStatus = INTERNAL_SERVER_ERROR;
	// cout << ports.size() << " | " << sockets.size() << endl;
	// cout << _requestHeaders["Host"] << endl;					no socket servers

	// string				requestHost = _requestHeaders["Host"];
	// uint16_t			requestPort = static_cast<uint16_t>(atoi(requestHost.substr(requestHost.find(":") + 1).c_str()));
	// vector<uint16_t>	matchedPortsIndex;
	// int					numServer = 0;

	//					server					listen					port
	// cout << this->get_value_block()[1]->get_value_block()[0]->get_value_inline()[2] << endl;

	std::vector<Directive *>	serverValueBlock = this->get_value_block(); // server blocks

	for (vector<Directive *>::iterator itServer = serverValueBlock.begin(); itServer != serverValueBlock.end(); ++itServer)
	{
		cout << (*itServer)->get_type() << endl; // => server
		// cout << (*itServer)->get_value_block()[0]->get_value_inline()[0] << endl; // => ports[0]

		std::vector<Directive *>	listenValueBlock = (*itServer)->get_value_block();
		// cout << listenValueBlock[0]->get_value_inline()[1] << endl; // => ports[1]

		for (size_t i = 0; i < listenValueBlock[0]->get_inline_size(); i++)
		{
			cout << listenValueBlock[0]->get_value_inline()[i] << endl;
		}

		// int i = 0;
		// for (vector<Directive *>::iterator itListen = listenValueBlock.begin(); itListen != listenValueBlock.end(); ++itListen)
		// {
		// 	cout << (*itListen)->get_type() << endl;
		// }

	}


	// cout << this->get_value_block()[0] << endl; // = 2

}

void Http::_send_response(Socket *client_socket)
{
	Log::response(_requestMethod, _request["httpVersion"], _request["uri"], _responseStatus);

	std::string response = _request["httpVersion"] + " " + _statusToString(_responseStatus)
						+ "\r\nContent-Type: text/html\r\n\r\n<html>";

	ssize_t bytes_sent = send(client_socket->get_socket(), response.c_str(), strlen(response.c_str()), 0);

	// TODO:
	// Se non si riesce ad inviare tutti i dati, impostare POLLOUT ed inviare
	// i dati rimanenti appena e' possibile scrivere.

	if (bytes_sent == -1) {
		Log::error("Sending the response failed");
		client_socket->close_socket();
	}
}

std::string Http::_statusToString(enum HTTP_STATUS status)
{
	switch (status)
	{
		case OK:
			return "200 OK";
		case BAD_REQUEST:
			return "400 Bad Request";
		case NOT_FOUND:
			return "404 Not Found";
		case INTERNAL_SERVER_ERROR:
			return "500 Internal Server Error";
		default:
			return "Unknown status";
	}
}

enum HTTP_METHOD Http::_methodToEnum(const std::string &method)
{
	if (method == "GET")
		return GET;
	else if (method == "POST")
		return POST;
	else if (method == "DELETE")
		return DELETE;
	else
		return UNKNOWN;
}

void Http::start_servers(void) {
	vector<uint16_t> ports = this->_get_ports();
	vector<Socket *> sockets;
	int num_ports = ports.size();
	struct pollfd fds[num_ports];
	int num_fds = 0;
	Socket *client_socket;
	std::string request;

	// Creates sockets and adds them to the pollfd structure ------------------>
	for (int i = 0; i < num_ports; i++) {
		sockets.push_back(new Socket(ports[i]));
		fds[num_fds].fd = sockets[i]->get_socket();
		fds[num_fds].events = POLLIN | POLLOUT; //monitora sia per lettura che per scrittura
		num_fds++;
	}

	// Polling ---------------------------------------------------------------->
	while (true) {
		int poll_status = poll(fds, num_fds, -1);
		if (poll_status == -1) {
			Log::error("Poll failed");
			exit(1);
		}
		// Handle events on sockets ------------------------------------------->
		for (int i = 0; i < num_fds; ++i) {
			if (fds[i].revents & POLLIN) {
				// Accept connection and create new socket for client---------->
				client_socket = sockets[i]->create_client_socket();

				// Read the request ------------------------------------------->
				request = _read_requests(client_socket);

				if (!request.empty())
				{
					// Parse the request ------------------------------------------>
					_parse_request(request);

					// Process the requests --------------------------------------->
					_process_requests();

					// Send the response ------------------------------------------>
					_send_response(client_socket);
				}

				// Close the client socket ------------------------------------>
				client_socket->close_socket();
			}

		}
	}
}
