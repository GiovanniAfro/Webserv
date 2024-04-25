/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:47:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:48:00 by kichkiro         ###   ########.fr       */
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
	* @note If the status is different from 200 OK, then try use error_page directive
*/
std::string Server::_getErrorPage(HTTP_STATUS status) {
	std::string errorPagePath = "", errorPageBody = "";
	if (status != OK) {
		ErrorPage *errorPage = static_cast<ErrorPage *>(this->getDirectives()["error_page"]);
		if (errorPage) {
			for (std::vector<HTTP_STATUS>::iterator it = errorPage->getCodes().begin(); it != errorPage->getCodes().end(); ++it) {
				if (status == *it) {
					Root *root = static_cast<Root *>(this->getDirectives()["root"]);
					std::string errorPagePath = root->getPath() + errorPage->getUri();
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
	response["body"] = status == OK ? body : _getErrorPage(status);
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
bool Server::_isMethodAllowed(const std::string &method, std::map<std::string, ADirective *> locaDirs) {
	if (locaDirs.find("limit_except") != locaDirs.end())
	{
		HTTP_METHOD locMethod = static_cast<LimitExcept *>(locaDirs["limit_except"])->getMethod();
		if (locMethod == Http::_methodToEnum(method))
			return true;
	}
	else if (method == "GET" || method == "POST" || method == "DELETE")
		return true;
	return false;
}

// Index and Autoindex ------------------------------------------------------>
/*!
	* @brief Check if the path is a folder.
	* @param path Path to the file.
	* @return bool True if the path is a folder, false otherwise.
*/
bool	Server::_isFolder(const std::string &path) {
	struct stat buffer;

	if (stat(path.c_str(), &buffer) == 0)
		return S_ISDIR(buffer.st_mode);
	return false;
}

/*!
	* @brief Check if the path is a file.
	* @param path Path to the file.
	* @return bool True if the path is a file, false otherwise.
*/
bool	Server::_isFile(const std::string &path) {
	struct stat buffer;

	if (stat(path.c_str(), &buffer) == 0)
		return S_ISREG(buffer.st_mode);
	return false;
}

/*!
	* @brief Check if the autoindex directive is set to on.
	* @param servDirs Server directives.
	* @param locaDirs Location directives.
	* @return bool True if the autoindex directive is defined and set to on, false otherwise.
*/
bool	Server::_isAutoIndex(std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs) {
	std::cout << "Checking autoindex" << std::endl;
	Autoindex *autoindex = static_cast<Autoindex *>(servDirs["autoindex"]);

	if (locaDirs.find("autoindex") != locaDirs.end())
	{
		std::cout << "Location autoindex" << std::endl;
		autoindex = static_cast<Autoindex *>(locaDirs["autoindex"]);
	}

	if (!autoindex)
		return false;
	return autoindex->getMode();
	(void)locaDirs;
}

/*!
	* @brief Get the index file.
	* @param filePath Path to the file.
	* @param servDirs Server directives.
	* @param locaDirs Location directives.
	* @return string Index file.
	* @note If no index file is found, return an empty string.
*/
std::string Server::_getIndex(std::string const &filePath, std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs) {
	Index *index = static_cast<Index *>(servDirs["index"]);

	if (locaDirs.find("index") != locaDirs.end())
		index = static_cast<Index *>(locaDirs["index"]);

	for (std::vector<std::string>::iterator it = index->getFiles().begin(); it != index->getFiles().end(); ++it)
		if (_isFile(filePath + *it))
			return *it;
	return "";
}

/*!
	* @brief Build the directory listing.
	* @param path Path to the directory.
	* @param uri URI.
	* @return map<string, string> Response map.
	* @note If the directory cannot be opened, return 500 Internal Server Error
*/
std::map<std::string, std::string> Server::_directoryListing(std::string const &path, std::string const &uri)
{
	std::map<std::string, std::string>	response;
	std::string							body;
	DIR									*dir;
	struct dirent						*ent;

	if ((dir = opendir(path.c_str())) != NULL)
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
	else
		response = _responseBuilder(INTERNAL_SERVER_ERROR);
	return response;
}
// -------------------------------------------------------------------------->

/*!
	* @brief Get the root directive value.
	* @param servDirs Server directives.
	* @param locaDirs Location directives.
	* @return string Root directive value
	* @note Order of operations:
			- If the root directive is defined in the location context, return the root
			- If the root directive is defined in the server context, return the root
			- If none of the above, return an empty string
*/
std::string Server::_getRoot(std::map<std::string, ADirective *> servDirs, std::map<std::string, ADirective *> locaDirs) {
	std::string root = "";

	if (locaDirs.find("root") != locaDirs.end())
		root = static_cast<Root *>(locaDirs["root"])->getPath();
	else if (servDirs.find("root") != servDirs.end())
		root = static_cast<Root *>(servDirs["root"])->getPath();
	return root;
}

std::map<std::string, std::string>	Server::processRequest(std::map<std::string, std::string> request) {
	std::map<std::string, ADirective *>	servDirs = this->getDirectives();
	std::map<std::string, ADirective *>	locaDirs;
	std::string path = "";
	std::string	requestUri = request["uri"];

	// Check if the request URI matches a location context
	Location *location = static_cast<Location *>(servDirs["location"]);
	for (std::vector<ADirective *>::iterator it = location->getBlocks().begin(); it != location->getBlocks().end(); ++it)
	{
		if (request["uri"].find(static_cast<Location *>(*it)->getUri()) != std::string::npos)
		{
			locaDirs = static_cast<Location *>(*it)->getDirectives();
			location = static_cast<Location *>(*it);
			break;
		}
	}

	// If the alias directive is defined, use the alias path
	// and append the request URI, minus the location URI to the path
	if (locaDirs.find("alias") != locaDirs.end())
	{
		path = static_cast<Alias *>(locaDirs["alias"])->getPath();
		requestUri = requestUri.substr(location->getUri().size());
	}
	else
		path = _getRoot(servDirs, locaDirs);
	if (path.empty())
		return _responseBuilder(INTERNAL_SERVER_ERROR);

	// Get the path to the file
	std::string	filePath = path + requestUri;
	if (!_isFile(filePath) && filePath[filePath.size() - 1] != '/')
		filePath += "/";

	// If the path is a folder, check for the index file and autoindex directive
	if (_isFolder(filePath))
	{
		if (servDirs.find("index") != servDirs.end())
		{
			std::string index = _getIndex(filePath, servDirs, locaDirs);
			std::cout << "Index: " << index << std::endl;
			if (index.empty() && _isAutoIndex(servDirs, locaDirs))
				return _directoryListing(filePath, request["uri"]);
			else if (index.empty())
				return _responseBuilder(FORBIDDEN);

			filePath += index;
		}
		else if (_isAutoIndex(servDirs, locaDirs))
			return _directoryListing(filePath, request["uri"]);
		else
			return _responseBuilder(FORBIDDEN);
	}
	else if (!_isFile(filePath))
		return _responseBuilder(NOT_FOUND);

	// Allowed methods
	if (!_isMethodAllowed(request.at("method"), locaDirs)) {
		return _responseBuilder(METHOD_NOT_ALLOWED);
	}

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
