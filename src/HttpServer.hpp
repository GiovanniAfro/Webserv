/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:05:36 by gcavanna          #+#    #+#             */
/*   Updated: 2024/02/28 15:08:31 by gcavanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

#include <iostream> // Per output/input di base.
#include <sys/socket.h> // Per le socket API.
#include <netinet/in.h> // Per la struttura `sockaddr_in`.
#include <unistd.h> // Per `read()`, `write()`, e `close()`.
#include <cstring> // Per `memset()`.
#include <cstdio>   // Per printf e perror
#include <cstdlib>  // Per exit e EXIT_FAILURE
#include <stdexcept>


class HttpServer
{
	private:
		int				   _server_fd;
		const int		   _PORT;
		int			       _addrlen; //Questa variabile rappresenta la dimensione della struttura address. È necessaria per funzioni come accept(), che hanno bisogno di sapere quanto grande è la struttura passata per poter copiare correttamente l'indirizzo del client che si sta connettendo. In pratica, informa il sistema operativo sulla quantità di memoria da considerare quando si lavora con l'indirizzo.
		struct sockaddr_in _address;
		const char 		  *_hello;

	public:
		HttpServer(int port);
		~HttpServer();

		void initSocket();
		void run();
};


#endif
