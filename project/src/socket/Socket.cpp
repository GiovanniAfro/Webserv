/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:32:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/11 12:30:52 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(uint16_t port) : _port(port) {
    this->_socket = this->_init_socket();
    this->_sock_addr.sin_family = AF_INET;
    this->_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_sock_addr.sin_port = htons(this->_port);
    this->_sock_addr_len = sizeof(this->_sock_addr);
    this->_type = "server";
    this->_binding();
    this->_listening();
}

Socket::Socket(int client_socket) : _socket(client_socket) {

    // this->_sock_addr.sin_family = AF_INET;
    // this->_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // this->_sock_addr.sin_port = htons(this->_port);
    this->_sock_addr_len = sizeof(this->_sock_addr);
    this->_type = "client";
    getpeername(client_socket, (struct sockaddr *)&this->_sock_addr, &this->_sock_addr_len);
    printf("Client connected from port %d\n", ntohs(this->_sock_addr.sin_port));
}

Socket::~Socket() {
    this->close_socket();
}

int Socket::_init_socket() {
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    return sock;
}

void Socket::_binding() {
    if (bind(this->_socket, (struct sockaddr *)&this->_sock_addr,
             sizeof(this->_sock_addr)) == -1) {
        perror("Binding failed");
        close(this->_socket);
        exit(1);
    }
}

void Socket::_listening() {
    if (listen(this->_socket, SOMAXCONN) == -1) {
        perror("Listening failed");
        close(this->_socket);
        exit(1);
    }
    cout << "Server listening on port " << ntohs(this->_sock_addr.sin_port) << endl;
}

Socket *Socket::create_client_socket() {
    int client_socket = accept(this->_socket, NULL, NULL);

    if (client_socket == -1) {
        perror("Acceptance failed");
        close(this->_socket);
        exit(1);
    }
    return new Socket(client_socket);
}

void Socket::close_socket() {
    close(this->_socket);
    cout << "socket " << this->_socket << " closed" << endl;
}

uint16_t Socket::get_port() {
    return this->_port;
}

int Socket::get_socket() {
    return this->_socket;
}

struct sockaddr_in Socket::get_sock_addr() {
    return this->_sock_addr;
}

socklen_t Socket::get_sock_addr_len() {
    return this->_sock_addr_len;
}

string Socket::get_type() {
    return this->_type;
}
