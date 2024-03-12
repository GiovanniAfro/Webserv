/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:47:13 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/12 17:31:46 by kichkiro         ###   ########.fr       */
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

Http::~Http(void) {
    for (VecDirIt it = this->_value_block.begin();
        it != this->_value_block.end(); ++it)
        delete *it;
}

vector<uint16_t> Http::_get_ports(void) {
    vector<uint16_t> ports;
    size_t num_servers = this->_value_block.size();
    size_t num_directives;
    size_t num_ports;
    uint16_t port;

    for (size_t i = 0; i < num_servers; i++) {
        num_directives = this->_value_block[i]->get_block_size();
        for (size_t j = 0; j < num_directives; j++) {
            if (this->_value_block[i]->get_value_block()[j]->get_type()
                == "listen") {
                num_ports = this->_value_block[i]->get_value_block()[j]
                    ->get_inline_size();
                for (size_t k = 0; k < num_ports; k++) {
                    if (this->_value_block[i]->get_value_block()[j]
                        ->get_value_inline()[k] != "default_server") {
                        port = static_cast<uint16_t>(atoi(
                            this->_value_block[i]->get_value_block()[j]
                            ->get_value_inline()[k].c_str()));
                        if (!uint16_t_in_vec(ports, port))
                            ports.push_back(port);
                    }
                }
            }
        }
    }
    return ports;
}


// DA SISTEMARE
string Http::_read_requests(Socket *client_socket) {
    char buf[1024];
    ssize_t bytes_read;
    string request = "";

    while ((bytes_read = recv(client_socket->get_socket(), buf,
                              sizeof(buf), MSG_DONTWAIT)) > 0) {
        buf[bytes_read] = 0;
        request.append(buf);
        bzero(buf, 1024);
    }
    if (!bytes_read)
        Log::info("Connection closed by remote host");
    // else if (bytes_read == -1) {
    //     Log::warning("No data available to read");
    //     // client_socket->close_socket();
    // }
    return request;
}

const char *Http::_process_requests(string request) {

    // Individuare il virtual server corretto ed inviargli la richiesta, 
    // quest'ultimo provvedera' a restituire la risposta

    // TMP
    Log::request(request);
    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html>";
}

void Http::_send_response(const char *response, Socket *client_socket) {
    ssize_t bytes_sent = send(client_socket->get_socket(), response,
                              strlen(response), 0);

    // TODO:
    // Se non si riesce ad inviare tutti i dati, impostare POLLOUT ed inviare 
    // i dati rimanenti appena e' possibile scrivere.

    if (bytes_sent == -1) {
        Log::error("Sending the response failed");
        client_socket->close_socket();
    }
}

void Http::start_servers(void) {
    vector<uint16_t> ports = this->_get_ports();
    vector<Socket *> sockets;
    int num_ports = ports.size();
    struct pollfd fds[num_ports];
    int num_fds = 0;
    Socket *client_socket;
    string request;
    const char *response;

    // Creates sockets and adds them to the pollfd structure ------------------>
    for (int i = 0; i < num_ports; i++) {
        sockets.push_back(new Socket(ports[i]));
        fds[num_fds].fd = sockets[i]->get_socket();
        fds[num_fds].events = POLLIN;
        num_fds++;
    }

    // Polling ---------------------------------------------------------------->
    while (true) {
        int poll_status = poll(fds, num_fds, -1);
        if (poll_status == -1) {
            Log::error("Poll failed");
            exit(1);
        }
        // Handle events on sockets ------------------------------------------->
        for (int i = 0; i < num_fds; ++i) {
            if (fds[i].revents & POLLIN) {

                // Accept connection and create new socket for client---------->
                client_socket = sockets[i]->create_client_socket();

                // Read the request ------------------------------------------->
                request = this->_read_requests(client_socket);

                // Process the requests --------------------------------------->
                response = this->_process_requests(request);

                // Send the response ------------------------------------------>
                this->_send_response(response, client_socket);

                // Close the client socket ------------------------------------>
                client_socket->close_socket();
            }

        }
    }
}
