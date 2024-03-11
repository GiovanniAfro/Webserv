/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:32:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/11 16:43:12 by kichkiro         ###   ########.fr       */
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
}

Socket::Socket(int client_socket) : _socket(client_socket) {
    this->_type = "client";
    this->_sock_addr_len = sizeof(this->_sock_addr);

    // Print client port
    if (getpeername(this->_socket, (struct sockaddr *)&this->_sock_addr, 
                    &this->_sock_addr_len) == -1) {
        perror("getpeername failed");
        return;
    }
    cout << endl << "Client connected from port: " <<
        ntohs(this->_sock_addr.sin_port) << endl << endl;
}

Socket::~Socket(void) {
    this->close_socket();
}

int Socket::_init_socket(void) {
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    return sock;
}

void Socket::_binding(void) {
    int opt_v = 1;

    // tells the socket to also use addresses that are in the TIME_WAIT state.
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt_v, sizeof(opt_v));

    if (bind(this->_socket, (struct sockaddr *)&this->_sock_addr,
             sizeof(this->_sock_addr)) == -1) {
        perror("Binding failed");
        close(this->_socket);
        exit(1);
    }
}

void Socket::_listening(void) {
    if (listen(this->_socket, SOMAXCONN) == -1) {
        perror("Listening failed");
        close(this->_socket);
        exit(1);
    }
    cout << "Server listening on port " << ntohs(this->_sock_addr.sin_port) <<
        endl;
}

Socket *Socket::create_client_socket(void) {
    int client_socket = accept(this->_socket, NULL, NULL);

    if (client_socket == -1) {
        perror("Acceptance failed");
        close(this->_socket);
        exit(1);
    }
    return new Socket(client_socket);
}

void Socket::close_socket(void) {
    close(this->_socket);
    cout << "socket fd: " << this->_socket << " - type: " << this->_type <<
        " closed" << endl;
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

string Socket::get_type(void) {
    return this->_type;
}
