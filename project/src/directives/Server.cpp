/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:47:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/01 11:34:18 by kichkiro         ###   ########.fr       */
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
std::map<std::string, std::string>	Server::_processGet(const std::string &filePath) {
	std::ifstream	file(filePath.c_str());

	if (file) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		return _responseBuilder(OK, buffer.str());
	}

	return _responseBuilder(NOT_FOUND);
}

std::map<std::string, std::string>	Server::_processPost(std::map<std::string, std::string> request, std::string const &filepath) {
	std::string fileNameHeader = "X-File-Name"; // Nome dell'header personalizzato per il nome del file

	// Controlla se l'header del nome del file e il Content-Type sono presenti
	if (request.find(fileNameHeader) != request.end() && request.find("Content-Type") != request.end()) {
		std::string fileName = request[fileNameHeader];
		std::string contentType = request["Content-Type"];
		std::string uploadDir = filepath + "/" + fileName; // Percorso dove salvare il file

		// Gestione basata sul Content-Type
		if (contentType == "text/plain" || contentType == "application/octet-stream") { // Esempi di Content-Type supportati
			std::ofstream outFile(uploadDir.c_str(), std::ofstream::binary);
			if (!outFile)
				return _responseBuilder(INTERNAL_SERVER_ERROR, "Impossibile aprire il file per la scrittura.");

			std::string	body = request.at("body");
			if (body.size() > static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max()))
				return _responseBuilder(INTERNAL_SERVER_ERROR, "File size is too large.");
			outFile.write(body.data(), static_cast<std::streamsize>(body.size()));
			outFile.close();

			return _responseBuilder(OK, "File caricato con successo.");
		}
		else
			return _responseBuilder(BAD_REQUEST, "Content-Type non supportato.");
	}
	else
		return _responseBuilder(BAD_REQUEST, "Header del nome del file mancante o Content-Type mancante.");
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
		std::cout << "HTTP error_page" << std::endl;

		if (_servDirs.find("error_page") != _servDirs.end())
			errorPage = static_cast<ErrorPage *>(_servDirs["error_page"]);
		std::cout << "Server error_page" << std::endl;

		if (_locaDirs.find("error_page") != _locaDirs.end())
			errorPage = static_cast<ErrorPage *>(_locaDirs["error_page"]);
		std::cout << "Location error_page" << std::endl;

		if (errorPage) {
			for (std::vector<HTTP_STATUS>::iterator it = errorPage->getCodes().begin(); it != errorPage->getCodes().end(); ++it) {
				if (status == *it) {
					std::string errorPagePath = _root + errorPage->getUri();
					std::cout << "Error page path: " << errorPagePath << std::endl;
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
	std::cout << "Checking autoindex" << std::endl;
	Autoindex *autoindex = static_cast<Autoindex *>(_httpDirs["autoindex"]);
	std::cout << "HTTP autoindex" << std::endl;

	if (_servDirs.find("autoindex") != _servDirs.end())
	{
		std::cout << "Server autoindex" << std::endl;
		autoindex = static_cast<Autoindex *>(_servDirs["autoindex"]);
	}

	if (_locaDirs.find("autoindex") != _locaDirs.end())
	{
		std::cout << "Location autoindex" << std::endl;
		autoindex = static_cast<Autoindex *>(_locaDirs["autoindex"]);
	}

	if (!autoindex)
		return false;

	std::cout << "Autoindex mode: " << autoindex->getMode() << std::endl;
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

std::map<std::string, std::string>	Server::processRequest(Http *http, Request clientRequest) {
	std::map<std::string, std::string> request = clientRequest.request;
	std::map<std::string, std::string> requestHeaders = clientRequest.requestHeaders;

	_httpDirs = http->getDirectives();
	_servDirs = this->getDirectives();
	std::string path = "", requestUri = request["uri"];

	// Check if the request URI matches a location context
	Location *location = static_cast<Location *>(_servDirs["location"]);
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
	std::cout << "File path: " << filePath << std::endl;

	// If the path is a folder, check for the index file and autoindex directive
	if (isFolder(filePath))
	{
		std::string index = _getIndex(filePath);
		std::cout << "Index: " << index << std::endl;

		if (index.empty() && _isAutoIndex())
			return _directoryListing(filePath, requestUri);
		else if (index.empty())
			return _responseBuilder(FORBIDDEN);

		filePath += isFolder(filePath) ? "/" + index : index;
	}
	else if (!isFile(filePath))
		return _responseBuilder(NOT_FOUND);

	// Allowed methods
	if (!_isMethodAllowed(request.at("method"))) {
		return _responseBuilder(METHOD_NOT_ALLOWED);
	}

	// CGI -------------------------------------------------------------------->
	std::cout << "here ------------------------------------------" << std::endl;
	Cgi(request, requestHeaders, filePath);
	// -------------------------------------------------------------------------


	try {
		if (request.at("method") == "GET")
			return _processGet(filePath);
		else if (request.at("method") == "POST")
			return _processPost(request, filePath);
		else if (request.at("method") == "DELETE")
			return _processDelete(filePath);
		else
			return _responseBuilder(BAD_REQUEST);
	}
	catch (const std::exception &ex) {
		return _responseBuilder(INTERNAL_SERVER_ERROR);
	}
}
