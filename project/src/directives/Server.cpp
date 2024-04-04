/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:47:42 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/04 16:32:38 by kichkiro         ###   ########.fr       */
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

Server::~Server() {
    for (VecDirIt it = this->_value_block.begin();
        it != this->_value_block.end(); ++it)
        delete *it;
}

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

map<string, string> Server::process_request(map<string, string> request) {

    // if (request["method"] == "GET")
    //     return this->_process_get(request);


    map<string, string> response;
    response["status"] = Http::statusToString(INTERNAL_SERVER_ERROR);


    // Processare la richiesta e ritornare la risposta in formato map o altra
    // struttura dati appropriata.

    return response;
    (void)request;
}
