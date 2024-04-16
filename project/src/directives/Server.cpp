/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:47:42 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/16 12:28:11 by gcavanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(string context) {
    vector<Directive *> value;

    if (context != "http")
        throw WrongContextExc("Server", "http", context);
    this->_type = "server";
    this->_is_context = true;
    this->_value_block = value;
}

Server::Server(ifstream &raw_value, string context) {
    if (context != "http")
        throw WrongContextExc("Server", "http", context);
    this->_type = "server";
    this->_is_context = true;
    this->_parsing_block(raw_value);
}

Server::~Server() {}

vector<Listen *> Server::get_listen(void) {
    vector<Listen *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "listen")
            result.push_back(dynamic_cast<Listen *>(directive));
    }
    return result;
}

vector<ServerName *> Server::get_server_name(void) {
    vector<ServerName *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "server_name")
            result.push_back(dynamic_cast<ServerName *>(directive));
    }
    return result;
}

vector<Root *> Server::get_root(void) {
    vector<Root *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "root")
            result.push_back(dynamic_cast<Root *>(directive));
    }
    return result;
}

vector<Alias *> Server::get_alias(void) {
    vector<Alias *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "alias")
            result.push_back(dynamic_cast<Alias *>(directive));
    }
    return result;
}

vector<Autoindex *> Server::get_autoindex(void) {
    vector<Autoindex *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "autoindex")
            result.push_back(dynamic_cast<Autoindex *>(directive));
    }
    return result;
}

vector<ErrorPage *> Server::get_error_page(void) {
    vector<ErrorPage *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "error_page")
            result.push_back(dynamic_cast<ErrorPage *>(directive));
    }
    return result;
}

vector<Rewrite *> Server::get_rewrite(void) {
    vector<Rewrite *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "rewrite")
            result.push_back(dynamic_cast<Rewrite *>(directive));
    }
    return result;
}

vector<ClientMaxBodySize *> Server::get_client_max_body_size(void) {
    vector<ClientMaxBodySize *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "client_max_body_size")
            result.push_back(dynamic_cast<ClientMaxBodySize *>(directive));
    }
    return result;
}

vector<Index *> Server::get_index(void) {
    vector<Index *> result;

    for (vector<Directive *>::const_iterator it = this->_value_block.begin();
         it != this->_value_block.end(); ++it) {
        Directive *directive = *it;

        if (directive->get_type() == "index")
            result.push_back(dynamic_cast<Index *>(directive));
    }
    return result;
}

/*!
    * @brief Process the GET request.
    * @param filePath Path to the file to be read.
    * @return map<string, string> Response map.
    * @note Verify if the file exists by checking file access.
    *       If the file is successfully read, return 200 OK
    *       If the file does not exist, return 404 Not Found
*/
map<string, string> Server::_process_get(const string &filePath)
{
    ifstream file(filePath.c_str());
    if (file)
    {
        stringstream buffer;
        buffer << file.rdbuf();
        return _responseBuilder(OK, buffer.str());
    }
    return _responseBuilder(NOT_FOUND);
}

map<string, string> Server::_process_post(map<string, string> request, string const &filepath)
{
    string fileNameHeader = "X-File-Name"; // Nome dell'header personalizzato per il nome del file

    // Controlla se l'header del nome del file e il Content-Type sono presenti
    if (request.find(fileNameHeader) != request.end() && request.find("Content-Type") != request.end())
    {
        string fileName = request[fileNameHeader];
        string contentType = request["Content-Type"];
        string uploadDir = filepath + "/" + fileName; // Percorso dove salvare il file

        // Gestione basata sul Content-Type
        if (contentType == "text/plain" || contentType == "application/octet-stream")
        { // Esempi di Content-Type supportati
            ofstream outFile(uploadDir.c_str(), ofstream::binary);
            if (!outFile)
                 return _responseBuilder(INTERNAL_SERVER_ERROR, "Impossibile aprire il file per la scrittura.");

            outFile.write(request.at("body").data(), request.at("body").size());
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
map<string, string> Server::_process_delete(const string &filePath)
{
    if (access(filePath.c_str(), F_OK) != -1)
    {
        if (remove(filePath.c_str()) == 0)
            _responseBuilder(OK);
        else
            _responseBuilder(INTERNAL_SERVER_ERROR);
    }

    return _responseBuilder(NOT_FOUND);
}

bool Server::_isFolder(const string &path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) == 0)
        return S_ISDIR(buffer.st_mode);
    return false;
}

// map<string, string> Server::_responseBuilder(HTTP_STATUS status, const string &body = "", const string &contentType = "text/html")
map<string, string> Server::_responseBuilder(HTTP_STATUS status, const string &body, const string &contentType)
{
    map<string, string> response;
    response["status"] = Http::statusToString(status);
    response["body"] = body;
    response["Content-Type"] = contentType;
    return response;
}

// Processare la richiesta e ritornare la risposta in formato map o altra
// struttura dati appropriata.
map<string, string> Server::process_request(map<string, string> request)
{
    try
    {
        map<string, string> response;
        string rootPath;

        // Trova il percorso root corretto
        vector<Root *> roots = get_root();
        if (!roots.empty())
        {
            // Prendi l'ultimo root disponibile
            rootPath = roots.back()->get_value_inline().front();
        }
        else
            return _responseBuilder(INTERNAL_SERVER_ERROR);

        // location = get_location("/test/")
        // location.directive['root']

        string filePath = rootPath + request.at("uri"); // Costruisci il percorso completo

        if (_isFolder(filePath))
            filePath += "/index.html";

        if (request.at("method") == "GET")
            return _process_get(filePath);
        else if (request.at("method") == "POST")
            return _process_post(request, filePath);
        else if (request.at("method") == "DELETE")
            return _process_delete(filePath);
        else
            return _responseBuilder(BAD_REQUEST);
    }
    catch(const std::exception& e)
    {
        return _responseBuilder(INTERNAL_SERVER_ERROR);
    }
}

