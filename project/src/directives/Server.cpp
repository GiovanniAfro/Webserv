/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:47:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/06 14:54:12 by gcavanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : ADirective("server", HTTP_CONTEXT) {}

Server::Server(const Server &copy) : ADirective(copy) {
	*this = copy;
}

Server::~Server() {}

Server &Server::operator=(const Server &other) {
	if (this != &other) {
		;
	}

	return *this;
}

ADirective *Server::clone() const {
	return new Server();
}


/*!
	* @brief Process the GET request.
	* @param filePath Path to the file to be read.
	* @return map<string, string> Response map.
	* @note Verify if the file exists by checking file access.
	*       If the file is successfully read, return 200 OK
	*       If the file does not exist, return 404 Not Found
*/

std::string Server::extractFileContent(const std::string& body, const std::string& boundary) {
    std::string boundaryDelimiter = "--" + boundary;
    size_t firstBoundaryPos = body.find(boundaryDelimiter);
    if (firstBoundaryPos == std::string::npos) {
        return "";  // Boundary non trovato
    }

    // Trova la fine dell'header del file
    size_t startContentPos = body.find("\r\n\r\n", firstBoundaryPos);
    if (startContentPos == std::string::npos) {
        return "";  // Fine dell'header non trovata
    }
    startContentPos += 4;  // Sposta l'indice oltre "\r\n\r\n"

    // Trova il prossimo boundary che segna la fine del contenuto del file
    size_t endContentPos = body.find(boundaryDelimiter, startContentPos);
    if (endContentPos == std::string::npos) {
        return "";  // Fine del contenuto non trovata
    }

    // Estrai il contenuto tra la fine dell'header e l'inizio del prossimo boundary
    return body.substr(startContentPos, endContentPos - startContentPos - 2);  // -2 per rimuovere il "\r\n" prima del boundary
}

std::string Server::extractFileName(const std::string& body, const std::string& boundary) {
    // Cerca il boundary nel corpo della richiesta
    std::string boundaryDelimiter = "--" + boundary;
    size_t boundaryPos = body.find(boundaryDelimiter);
    if (boundaryPos == std::string::npos) {
        return "";  // Boundary non trovato, impossibile analizzare la richiesta
    }

    // Trova la prima occorrenza di "Content-Disposition" dopo il boundary
    size_t dispositionPos = body.find("Content-Disposition: form-data;", boundaryPos);
    if (dispositionPos == std::string::npos) {
        return "";  // "Content-Disposition" non trovato
    }

    // Cerca "filename=" che dovrebbe seguire il "Content-Disposition"
    size_t filenamePos = body.find("filename=\"", dispositionPos);
    if (filenamePos == std::string::npos) {
        return "";  // "filename=" non trovato
    }
    filenamePos += 10;  // Salta la lunghezza di "filename=\""

    // Trova la fine del nome del file
    size_t filenameEndPos = body.find("\"", filenamePos);
    if (filenameEndPos == std::string::npos) {
        return "";  // Terminatore del nome del file non trovato
    }

    // Estrai e restituisci il nome del file
    return body.substr(filenamePos, filenameEndPos - filenamePos);
}

std::string Server::extractBoundary(const std::string& contentType) {
    std::string boundaryPrefix = "boundary=";
    size_t startPos = contentType.find(boundaryPrefix);
    if (startPos == std::string::npos) {
        return ""; // Boundary non trovato
    }
    
    startPos += boundaryPrefix.length(); // Sposta l'indice all'inizio del valore di boundary
    size_t endPos = contentType.find(";", startPos); // Cerca la fine del valore di boundary
    if (endPos == std::string::npos) {
        endPos = contentType.length(); // Se non c'è ';', il boundary va fino alla fine della stringa
    }

    // Rimuovi gli spazi bianchi all'inizio e alla fine del boundary
    while (startPos < endPos && std::isspace(contentType[startPos])) {
        startPos++;
    }
    while (endPos > startPos && std::isspace(contentType[endPos - 1])) {
        endPos--;
    }

    return contentType.substr(startPos, endPos - startPos);
}

std::map<std::string, std::string>	Server::_processGet(const std::string &filePath) {
	std::ifstream	file(filePath.c_str());

	if (file) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		return _responseBuilder(OK, buffer.str());
	}

	return _responseBuilder(NOT_FOUND);
}

std::map<std::string, std::string> Server::_processPost(Request clientRequest, std::string const &filepath)
{
    std::map<std::string, std::string> requestHeaders = clientRequest.requestHeaders;

    std::string contentType = requestHeaders["Content-Type"];

    // Gestisci i diversi tipi di Content-Type
    if (contentType.find("multipart/form-data") != std::string::npos) {
        std::string boundary = extractBoundary(contentType);
        if (boundary.empty()) {
            return _responseBuilder(BAD_REQUEST, "Boundary non trovato nel Content-Type.");
        }

        std::string fileName = extractFileName(clientRequest.requestBody, boundary);
        if (fileName.empty()) {
            return _responseBuilder(BAD_REQUEST, "Nome del file mancante nel corpo della richiesta.");
        }

        std::string fullPath = filepath + fileName;
        std::string fileContent = extractFileContent(clientRequest.requestBody, boundary);
        if (fileContent.empty()) {
            return _responseBuilder(BAD_REQUEST, "Contenuto del file mancante.");
        }

        // Verifica la sicurezza del path del file
        if (fileName.find("..") != std::string::npos) {
            return _responseBuilder(BAD_REQUEST, "Percorso del file non valido.");
        }

        std::ofstream outFile(fullPath.c_str(), std::ofstream::binary);
        if (!outFile) {
            return _responseBuilder(INTERNAL_SERVER_ERROR, "Impossibile aprire il file per la scrittura.");
        }

        outFile.write(fileContent.data(), fileContent.size());
        outFile.close();

    } else if (contentType.find("text/plain") != std::string::npos) {
        // Qui puoi gestire il testo semplice, ad esempio salvandolo in un file o facendo qualcos'altro.
        std::string fullPath = filepath + "some_default_filename.txt";
        std::ofstream outFile(fullPath.c_str(), std::ofstream::binary);
        if (!outFile) {
            return _responseBuilder(INTERNAL_SERVER_ERROR, "Impossibile aprire il file per la scrittura.");
        }
        outFile << clientRequest.requestBody;
        outFile.close();
    } else {
        return _responseBuilder(BAD_REQUEST, "Content-Type non supportato.");
    }

    return _responseBuilder(OK, "Richiesta gestita con successo.");
}


/*!
	* @brief Process the DELETE request.
	* @param filePath Path to the file to be deleted.
	* @return map<string, string> Response map.
	* @note Verify if the file exists by checking file access.
	*       If the file is successfully deleted, return 200 OK
	*       If the file does not exist, return 404 Not Found
	*       If the file cannot be deleted, return 500 Internal Server Error
*/
std::map<std::string, std::string>	Server::_processDelete(const std::string &filePath) {
	if (access(filePath.c_str(), F_OK) != -1) {
		if (remove(filePath.c_str()) == 0)
			_responseBuilder(OK);
		else
			_responseBuilder(INTERNAL_SERVER_ERROR);
	}

	return _responseBuilder(NOT_FOUND);
}

/*!
	* @brief Get the error page.
	* @param status HTTP_STATUS enum.
	* @return string Error page.
	* @note If the status is different from 200 OK, then try use error_page directive:
			- If the error_page directive is defined in the location context, and the
			  request matches the location URI, return the error page
			- If the error_page directive is defined in the server context, return the error page
			- If the error_page directive is defined in the http context, return the error page
			- If none of the above, return the status message
*/
std::string Server::_getErrorPage(HTTP_STATUS status) {
	std::string errorPagePath = "", errorPageBody = "";
	if (status != OK) {
		ErrorPage *errorPage = static_cast<ErrorPage *>(_httpDirs["error_page"]);

		if (_servDirs.find("error_page") != _servDirs.end())
			errorPage = static_cast<ErrorPage *>(_servDirs["error_page"]);

		if (_locaDirs.find("error_page") != _locaDirs.end())
			errorPage = static_cast<ErrorPage *>(_locaDirs["error_page"]);

		if (errorPage) {
			for (std::vector<HTTP_STATUS>::iterator it = errorPage->getCodes().begin(); it != errorPage->getCodes().end(); ++it) {
				if (status == *it) {
					std::string errorPagePath = _root + errorPage->getUri();
					if (!errorPagePath.empty()) {
						std::ifstream	file(errorPagePath.c_str());
						if (file) {
							std::stringstream buffer;
							buffer << file.rdbuf();
							errorPageBody = buffer.str();
						}
					}
				}
			}
		}
	}

	if (errorPageBody.empty() && status != OK)
		return Http::_statusToMessage(status);
	return errorPageBody;
}

/*!
	* @brief Build the response map.
	* @param status HTTP_STATUS enum.
	* @param body Response body.
	* @param contentType Content-Type header.
	* @return map<string, string> Response map.
	* @note If the status is different from 200 OK, then try use error_page directive
*/
std::map<std::string, std::string>	Server::_responseBuilder(HTTP_STATUS status, const std::string &body, const std::string &contentType) {
	std::map<std::string, std::string> response;
	response["status"] = Http::_statusToString(status);
	switch (status)
	{
		case OK:
			response["body"] = body;
			break;
		case FOUND:
			response["Location"] = body;
			break;
		default:
			response["body"] = _getErrorPage(status);
			break;
	}
	response["Content-Type"] = contentType;
	return response;
}

/*!
	* @brief Check if the method is allowed.
	* @param method HTTP method.
	* @return bool True if the method is allowed, false otherwise.
	* @note In case the request contains a location context, check if the method is allowed
			by the limit_except directive, otherwise check if the method is GET, POST or DELETE
*/
bool Server::_isMethodAllowed(const std::string &method) {
	if (_locaDirs.find("limit_except") != _locaDirs.end())
	{
		HTTP_METHOD locMethod = static_cast<LimitExcept *>(_locaDirs["limit_except"])->getMethod();
		if (locMethod == Http::_methodToEnum(method))
			return true;
	}
	else if (method == "GET" || method == "POST" || method == "DELETE")
		return true;
	return false;
}

// Index and Autoindex ------------------------------------------------------>
/*!
	* @brief Check if the autoindex directive is set to on.
	* @return bool True if the autoindex directive is defined and set to on, false otherwise.
*/
bool	Server::_isAutoIndex() {
	Autoindex *autoindex = static_cast<Autoindex *>(_httpDirs["autoindex"]);

	if (_servDirs.find("autoindex") != _servDirs.end())
		autoindex = static_cast<Autoindex *>(_servDirs["autoindex"]);

	if (_locaDirs.find("autoindex") != _locaDirs.end())
		autoindex = static_cast<Autoindex *>(_locaDirs["autoindex"]);

	if (!autoindex)
		return false;

	return autoindex->getMode();
}

/*!
	* @brief Get the index file.
	* @param filePath Path to the file.
	* @return string Index file.
	* @note If no index file is found, return an empty string.
*/
std::string Server::_getIndex(std::string const &filePath) {
	Index *index = static_cast<Index *>(_httpDirs["index"]);
	std::string path = isFolder(filePath) ? filePath + "/" : filePath;

	if (_servDirs.find("index") != _servDirs.end())
		index = static_cast<Index *>(_servDirs["index"]);

	if (_locaDirs.find("index") != _locaDirs.end())
		index = static_cast<Index *>(_locaDirs["index"]);

	if (!index)
		return "";

	for (std::vector<std::string>::iterator it = index->getFiles().begin(); it != index->getFiles().end(); ++it)
		if (isFile(path + *it))
			return *it;
	return "";
}

/*!
	* @brief Build the directory listing.
	* @param filePath Path to the directory.
	* @param uri URI.
	* @return map<string, string> Response map.
	* @note If the directory cannot be opened, return 500 Internal Server Error
*/
std::map<std::string, std::string> Server::_directoryListing(std::string const &filePath, std::string const &uri)
{
	std::string							body;
	DIR									*dir;
	struct dirent						*ent;

	if ((dir = opendir(filePath.c_str())) != NULL)
	{
		body += "<html><head><title>Directory listing</title></head><body><h1>Directory listing</h1><ul>";
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_name[0] == '.')
				continue;
			body += "<li><a href=\"";
			if (uri[uri.size() - 1] != '/')
			{
				body += uri;
				body += "/";
			}
			body += ent->d_name;
			body += "\">";
			body += ent->d_name;
			body += "</a></li>";
		}
		body += "</ul></body></html>";
		closedir(dir);

		return _responseBuilder(OK, body, "text/html");
	}
	return _responseBuilder(INTERNAL_SERVER_ERROR);
}
// -------------------------------------------------------------------------->

/*!
	* @brief Get the root directive value.
	* @return string Root directive value
	* @note Order of operations:
			- If the root directive is defined in the location context, return the root
			- If the root directive is defined in the server context, return the root
			- If the root directive is defined in the http context, return the root
			- If none of the above, return an empty string
*/
std::string Server::_getRoot() {
	std::string root = "";

	if (_locaDirs.find("root") != _locaDirs.end())
		root = static_cast<Root *>(_locaDirs["root"])->getPath();
	else if (_servDirs.find("root") != _servDirs.end())
		root = static_cast<Root *>(_servDirs["root"])->getPath();
	else if (_httpDirs.find("root") != _httpDirs.end())
		root = static_cast<Root *>(_httpDirs["root"])->getPath();
	return root;
}

/*!
	* @brief Get the client_max_body_size directive value.
	* @param request Request map.
	* @param requestHeaders Request headers map.
	* @return bool True if the body size is exceeded, false otherwise.
	* @note Check if the body size is exceeded in the following order:
			- If the client_max_body_size directive is defined in the location context
			- If the client_max_body_size directive is defined in the server context
			- If the Content-Length header is defined in the request
			- If the content length is greater than the client_max_body_size
*/
bool Server::_isBodySizeExceeded(std::map<std::string, std::string> request, std::map<std::string, std::string> requestHeaders)
{
	unsigned long long maxBodySize = DEFAULT_MAX_BODY_SIZE;

	if (_locaDirs.find("client_max_body_size") != _locaDirs.end())
		maxBodySize = static_cast<ClientMaxBodySize *>(_locaDirs["client_max_body_size"])->getSize();
	else if (_servDirs.find("client_max_body_size") != _servDirs.end())
		maxBodySize = static_cast<ClientMaxBodySize *>(_servDirs["client_max_body_size"])->getSize();

	unsigned long long contentLength = request["body"].size();
	if (requestHeaders.find("Content-Length") != request.end())
		contentLength = strtoull(requestHeaders["Content-Length"].c_str(), NULL, 10);

	return (contentLength > maxBodySize);
}

/*!
	* @brief Get the rewrite directive value.
	* @param requestUri Request URI.
	* @return string Rewrite directive value.
	* @note Order of operations:
			- If the rewrite directive is defined in the location context and matches the URI,
				return the replacement URI
			- If the rewrite directive is defined in the server context and matches the URI,
				return the replacement URI
			- If none of the above, return the request URI
*/
std::string Server::_getRewrite(std::string const &requestUri) {
	Rewrite *rewrite = static_cast<Rewrite *>(_servDirs["rewrite"]);
	if (_locaDirs.find("rewrite") != _locaDirs.end())
		rewrite = static_cast<Rewrite *>(_locaDirs["rewrite"]);

	if (requestUri != "" && requestUri != "/"
		&& requestUri.find(rewrite->getUri()) != std::string::npos)
	{
		std::string tmp = strRemove(_root + requestUri, rewrite->getUri());
		if (rewrite->getReplacement()[0] != '/')
			tmp = tmp + "/" + rewrite->getReplacement();

		if (isFile(tmp))
			return strReplace(requestUri, rewrite->getUri(), "/" + rewrite->getReplacement());
		else if (isFolder(tmp))
			return strReplace(requestUri, rewrite->getUri(), rewrite->getReplacement());
	}
	return requestUri;
}

std::map<std::string, std::string>	Server::processRequest(Http *http, Request clientRequest)
{
	std::map<std::string, std::string> request = clientRequest.request;
	std::map<std::string, std::string> requestHeaders = clientRequest.requestHeaders;

	_httpDirs = http->getDirectives();
	_servDirs = this->getDirectives();
	std::string path = "", requestUri = request["uri"];
	
	std::map<std::string, ADirective*>::iterator locIt = _servDirs.find("location");
	if (locIt == _servDirs.end()) {
    // Gestisci l'errore: "location" non trovata
    	return _responseBuilder(INTERNAL_SERVER_ERROR, "Location directive not found");
	}

	Location* location = static_cast<Location*>(locIt->second);
	if (!location) {
    // Gestisci l'errore: il cast non è andato a buon fine
    	return _responseBuilder(INTERNAL_SERVER_ERROR, "Failed to cast to Location type");
	}

	// Check if the request URI matches a location context
	for (std::vector<ADirective *>::iterator it = location->getBlocks().begin(); it != location->getBlocks().end(); ++it)
	{
		if (requestUri.find(static_cast<Location *>(*it)->getUri()) != std::string::npos)
		{
			_locaDirs = static_cast<Location *>(*it)->getDirectives();
			location = static_cast<Location *>(*it);
			break;
		}
	}

	// Check if the body size of the request is exceeded, in case of a POST request
	if (request["method"] == "POST" && _isBodySizeExceeded(request, requestHeaders))
		return _responseBuilder(PAYLOAD_TOO_LARGE);

	// If the alias directive is defined, use the alias path
	// and append the request URI, minus the location URI to the path
	if (_locaDirs.find("alias") != _locaDirs.end())
	{
		_root = static_cast<Alias *>(_locaDirs["alias"])->getPath();
		requestUri = requestUri.substr(location->getUri().size());
	}
	else
		_root = _getRoot();
	if (_root.empty())
		return _responseBuilder(INTERNAL_SERVER_ERROR);

	// Check if the request URI matches a rewrite directive
	requestUri = _getRewrite(request["uri"]);
	if (requestUri != request["uri"])
		return _responseBuilder(FOUND, requestUri);

	// Get the path to the file
	std::string	filePath = _root + requestUri;
	if (!isFile(filePath) && filePath[filePath.size() - 1] != '/')
		filePath += "/";
	else if (filePath[filePath.size() - 1] == '/')
		filePath = filePath.substr(0, filePath.size() - 1);



	// If the path is a folder, check for the index file and autoindex directive
	if (isFolder(filePath) && request["method"] == "GET")
	{
		std::string index = _getIndex(filePath);

		if (index.empty() && _isAutoIndex())
			return _directoryListing(filePath, requestUri);
		else if (index.empty())
			return _responseBuilder(FORBIDDEN);

		filePath += isFolder(filePath) ? "/" + index : index;
	}
	else if (!isFile(filePath) && request["method"] == "GET")
		return _responseBuilder(NOT_FOUND);

	// Allowed methods
	if (!_isMethodAllowed(request.at("method"))) {
		return _responseBuilder(METHOD_NOT_ALLOWED);
	}

	// CGI -------------------------------------------------------------------->
	// for (std::map<std::string, ADirective *>::iterator it = _locaDirs.begin(); it != _locaDirs.end(); ++it)
	// 	std::cout << "Location directive: " << it->first << std::endl;

	// std::cout << "here ------------------------------------------" << std::endl;
	if (_locaDirs.find("fastcgi_pass") != _locaDirs.end())
	{
		Cgi cgi(clientRequest, filePath);
		return _responseBuilder(OK, cgi.exec());
	}

	try {
		if (request.at("method") == "GET")
			return _processGet(filePath);
		else if (request.at("method") == "POST")
			return _processPost(clientRequest, filePath);
		else if (request.at("method") == "DELETE")
			return _processDelete(filePath);
		else
			return _responseBuilder(BAD_REQUEST);
	}
	catch (const std::exception &ex) {
		return _responseBuilder(INTERNAL_SERVER_ERROR);
	}
}
