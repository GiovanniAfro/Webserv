/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:47:42 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/09 17:48:23 by gcavanna         ###   ########.fr       */
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

map<string, string> Server::_process_get(const string &filePath)
{
    map<string, string> response;
    ifstream file(filePath.c_str()); // Apri qui il file

    if (file)
    {
        stringstream buffer;
        buffer << file.rdbuf(); // Leggi il contenuto
        response["body"] = buffer.str();
        response["Content-Type"] = "text/html";
        response["status"] = Http::statusToString(OK);
    }
    else
        response["status"] = Http::statusToString(NOT_FOUND);

    return response;
}


/* map<string, string> Server::_process_post(map<string, string> request)
{
    map<string, string> response;
    response["status"] = Http::statusToString(INTERNAL_SERVER_ERROR);
    return response;
}
*/
map<string, string> Server::_process_delete(const string &filePath)
{
    map<string, string> response;

    // Verifica se il file esiste verificando l'accesso al file
    if (access(filePath.c_str(), F_OK) != -1)
    {
        // Prova ad eliminare il file
        if (remove(filePath.c_str()) == 0)
        {
            // Se la rimozione è riuscita, restituisci 200 OK
            response["status"] = Http::statusToString(OK);
        } 
        else
        {
            // Se la rimozione fallisce, restituisci 500 Internal Server Error
            response["status"] = Http::statusToString(INTERNAL_SERVER_ERROR);
        }
    }
    else 
    {
        // Se il file non esiste, restituisci 404 Not Found
        response["status"] = Http::statusToString(NOT_FOUND);
    }

    return response;
}


map<string, string> Server::_process_unknown(void)
{
    map<string, string> response;
    response["status"] = Http::statusToString(BAD_REQUEST);
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
            cout << "Root path trovato: " << rootPath << endl;
        }
        else
        {
            // Nessun root definito
            cout << "Nessun root definito per il server." << endl;
            response["status"] = Http::statusToString(INTERNAL_SERVER_ERROR);
            return response;
        }

        string filePath = "." + rootPath + request.at("uri"); // Costruisci il percorso completo
        cout << "Percorso file completo: " << filePath << endl;

        // Apri il file
        ifstream file(filePath.c_str());
        if (!file)
        {
            cout << "File non trovato: " << filePath << endl;
            response["status"] = Http::statusToString(NOT_FOUND); // File non trovato
            return response;
        } 

        if (request.at("method") == "GET")
        {
            cout << "Gestione richiesta GET per: " << filePath << endl;
            return _process_get(filePath); // Passa il percorso del file
        }
        /*else if (request.at("method") == "POST")
            return _process_post(request);
        */
        else if (request.at("method") == "DELETE")
            return _process_delete(filePath);
        else
        {
            cout << "Metodo richiesta sconosciuto o non supportato." << endl;
            return this->_process_unknown(); 
        }
    }
    catch(const std::exception& e)
    {
        cout << "Errore durante l'elaborazione della richiesta: " << e.what() << endl;
        map<string, string> response;
        response["status"] = Http::statusToString(INTERNAL_SERVER_ERROR);
        return response;
    }
}

