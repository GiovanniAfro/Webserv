/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:32:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/12 17:22:49 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(uint16_t port) : _port(port) {
    this->_type = "server";
    this->_socket = this->_init_socket();
    this->_sock_addr.sin_family = AF_INET;
    this->_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_sock_addr.sin_port = htons(this->_port);
    this->_sock_addr_len = sizeof(this->_sock_addr);
    this->_binding();
    this->_listening();
    this->_set_non_blocking();
}

Socket::Socket(int client_socket) : _socket(client_socket) {
    this->_type = "client";
    this->_sock_addr_len = sizeof(this->_sock_addr);
    // this->_set_non_blocking();
}

Socket::~Socket(void) {
    this->close_socket();
}

int Socket::_init_socket(void) {
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        Log::error("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void Socket::_binding(void) {
    int opt_v = 1;

    // tells the socket to also use addresses that are in the TIME_WAIT state.
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt_v, sizeof(opt_v));

    if (bind(this->_socket, (struct sockaddr *)&this->_sock_addr,
             sizeof(this->_sock_addr)) == -1) {
        Log::error("Binding failed");
        close(this->_socket);
        exit(EXIT_FAILURE);
    }
}

void Socket::_listening(void) {
    if (listen(this->_socket, SOMAXCONN) == -1) {
        Log::error("Listening failed");
        close(this->_socket);
        exit(EXIT_FAILURE);
    }
    stringstream ss;
    ss << ntohs(this->_sock_addr.sin_port);
    Log::info("Server listening on port " + ss.str());
}

void Socket::_set_non_blocking(void) {
    this->_flags = this->get_flags();
    if (fcntl(this->_socket, F_SETFL, this->_flags | O_NONBLOCK) == -1) {
        Log::error("Failed to set socket non-blocking");
        exit(EXIT_FAILURE);
    }
    this->_flags = this->get_flags();
}

Socket *Socket::create_client_socket(void) {
    int client_socket = accept(this->_socket, NULL, NULL);

    if (client_socket == -1) {
        Log::error("Acceptance failed");
        close(this->_socket);
        exit(EXIT_FAILURE);
    }
    return new Socket(client_socket);
}

void Socket::close_socket(void) {
    close(this->_socket);
}

string Socket::get_type(void) {
    return this->_type;
}

uint16_t Socket::get_port(void) {
    return this->_port;
}

int Socket::get_socket(void) {
    return this->_socket;
}

struct sockaddr_in Socket::get_sock_addr(void) {
    return this->_sock_addr;
}

socklen_t Socket::get_sock_addr_len(void) {
    return this->_sock_addr_len;
}

int Socket::get_flags(void) {
    int flags = fcntl(this->_socket, F_GETFL, 0);

    if (flags == -1) {
        Log::error("Failed to get socket flags");
        exit(EXIT_FAILURE);
    }
    return flags;
}
