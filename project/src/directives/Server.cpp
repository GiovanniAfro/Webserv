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
	* @brief Build the response map.
	* @param status HTTP_STATUS enum.
	* @param body Response body.
	* @param contentType Content-Type header.
	* @return map<string, string> Response map.
*/
std::map<std::string, std::string>	Server::_responseBuilder(HTTP_STATUS status, const std::string &body, const std::string &contentType) {
	std::map<std::string, std::string> response;
	response["status"] = Http::_statusToString(status);
	response["body"] = body;
	response["Content-Type"] = contentType;
	return response;
}

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
	* @brief Check if the method is allowed.
	* @param method HTTP method.
	* @return bool True if the method is allowed, false otherwise.
*/
bool Server::_isMethodAllowed(const std::string &method) {
	if (method == "GET" || method == "POST" || method == "DELETE")
		return true;
	return false;
}

/*!
	* @brief Get the index file.
	* @return string Index file.
	* @note If the index directive is not found, return an empty string.
*/
std::string Server::_getIndex() {
	std::map<std::string, ADirective *>	directives = this->getDirectives();
	if (directives.find("index") == directives.end())
		return "";
	return "";

	// Index *index = static_cast<Index*>(directives["index"]);
	// if (index->getValues().empty())
	// 	return "";
	// return index->getValues()[0];
}

std::map<std::string, std::string>	Server::processRequest(std::map<std::string, std::string> request) {
	std::map<std::string, ADirective *>	directives = this->getDirectives();
	// for (std::map<std::string, ADirective*>::iterator it = directives.begin(); it != directives.end(); ++it)
	// 	std::cout << it->first << std::endl;

	if (directives.find("root") == directives.end())
		return _responseBuilder(INTERNAL_SERVER_ERROR);

	std::string	rootValue = static_cast<Root *>(this->getDirectives()["root"])->getPath();

	if (rootValue.empty())
		return _responseBuilder(INTERNAL_SERVER_ERROR);

	std::string	requestUri = request["uri"];
	std::string	filePath = rootValue + requestUri;

	if (_isFolder(filePath)) {
		std::string index = _getIndex();
		if (index == "")
			return _responseBuilder(INTERNAL_SERVER_ERROR);

		filePath += index;
	}

	// Allowed methods
	if (!_isMethodAllowed(request.at("method"))) {
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
