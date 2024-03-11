/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:47:13 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/11 12:48:26 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(string context) {
    vector<Directive *> value;

    if (context != "main")
        throw WrongContextExc("Http", "main", context);
    this->_type = "http";
    this->_is_context = true;
    this->_value_block = value;
}

Http::Http(ifstream &raw_value, string context) {
    if (context != "main")
        throw WrongContextExc("Http", "main", context);
    this->_type = "http";
    this->_is_context = true;
    this->_parsing_block(raw_value);
}

Http::~Http() {
    for (VecDirIt it = this->_value_block.begin();
        it != this->_value_block.end(); ++it)
        delete *it;
}

vector<int> Http::_get_ports(void) {
    vector<int> ports;
    size_t num_servers = this->_value_block.size();
    size_t num_directives;
    size_t num_ports;
    int port;

    for (size_t i = 0; i < num_servers; i++) {
        num_directives = this->_value_block[i]->get_block_size();
        for (size_t j = 0; j < num_directives; j++) {
            if (this->_value_block[i]->get_value_block()[j]->get_type()
                == "listen") {
                num_ports = this->_value_block[i]->get_value_block()[j]->get_inline_size();
                for (size_t k = 0; k < num_ports; k++) {
                    if (this->_value_block[i]->get_value_block()[j]->get_value_inline()[k] != "default_server") {
                        port = atoi(this->_value_block[i]->get_value_block()[j]->get_value_inline()[k].c_str());
                        if (!int_in_vec(ports, port))
                            ports.push_back(port);
                    }
                }
            }
        }
    }
    return ports;
}

string Http::_read_requests(Socket *client_socket) {
    char buffer[1024];
    ssize_t bytes_read = read(client_socket->get_socket(), buffer, sizeof(buffer));

    if (bytes_read == -1) {
        perror("Reading failed");
        client_socket->close_socket();
    }
    return string(buffer, bytes_read);
}

const char *Http::_process_requests(string request) {

    // Individuare il virtual server corretto ed inviargli la richiesta, 
    // quest'ultimo provvedera' a restituire la risposta

    // TMP
    cout << "HTTP request received: " << request << endl;
    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html>";
}

void Http::_send_response(const char *response, Socket *client_socket) {
    ssize_t bytes_sent = send(client_socket->get_socket(), response, strlen(response), 0);

    if (bytes_sent == -1) {
        perror("Sending the response failed");
        client_socket->close_socket();
    }
}

void Http::start_servers() {
    vector<int> ports = this->_get_ports();
    int num_ports = ports.size();
    vector<Socket *> sockets;
    struct pollfd fds[num_ports];
    int num_fds = 0;

    // Creates sockets and adds them to the pollfd structure ------------------>
    for (int i = 0; i < num_ports; i++) {
        sockets.push_back(new Socket(static_cast<uint16_t>(ports[i])));
        cout << sockets[i]->get_socket() << endl;
        fds[num_fds].fd = sockets[i]->get_socket();
        fds[num_fds].events = POLLIN;
        num_fds++;
    }

    // Polling ---------------------------------------------------------------->
    while (true) {
        int poll_status = poll(fds, num_fds, -1);
        if (poll_status == -1) {
            perror("Poll failed");
            exit(1);
        }
        // Handle events on sockets ------------------------------------------->
        for (int i = 0; i < num_fds; ++i) {
            if (fds[i].revents & POLLIN) {

                // Accept connection and create new socket for client---------->
                Socket *client_socket = sockets[i]->create_client_socket();

                // Read the request ------------------------------------------->
                string request = this->_read_requests(client_socket);

                // Process the requests --------------------------------------->
                const char *response = this->_process_requests(request);

                // Send the request ------------------------------------------->
                this->_send_response(response, client_socket);

                client_socket->close_socket();
            }
        }
    }
}
