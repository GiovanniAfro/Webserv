/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:31:03 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/12 13:01:14 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

class Socket {
    private:
        string             _type;
        uint16_t           _port;
        int                _socket;
        struct sockaddr_in _sock_addr;
        socklen_t          _sock_addr_len;
        int                _flags;

        int  _init_socket(void);
        void _binding(void);
        void _listening(void);
        void _set_non_blocking(void);

    public:

        Socket(uint16_t port);     // for server socket
        Socket(int client_socket); // for client socket
        ~Socket(void);

        string             get_type(void);
        uint16_t           get_port(void);
        int                get_socket(void);
        struct sockaddr_in get_sock_addr(void);
        socklen_t          get_sock_addr_len(void);
        int                get_flags(void);

        Socket *create_client_socket(void);
        void    close_socket(void);
};
