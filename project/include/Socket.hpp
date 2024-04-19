/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:31:03 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/18 17:13:10 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

class Socket {
    private:
		const std::string	_type;
		uint16_t			_port;
		int					_socket;
		struct sockaddr_in	_sockAddr;
		socklen_t			_sockAddrLen;
		int					_flags;

		int  _socketInnit();
		int _binding();
		int _listening();
		int _setNonBlocking();

	public:

		Socket(uint16_t port);     // for server socket
		Socket(int client_socket); // for client socket
		~Socket(void);

        const std::string&	getType(void);
        uint16_t			getPort(void);
        int					getSocket(void);
        struct sockaddr_in	getSockAddr(void);
        socklen_t			getSockAddrLen(void);
        int					getFlags(void);

        Socket*	createClientSocket(void);
        void	closeSocket(void);
};
