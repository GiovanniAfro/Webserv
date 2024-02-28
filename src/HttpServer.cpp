/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:09:32 by gcavanna          #+#    #+#             */
/*   Updated: 2024/02/28 15:39:06 by gcavanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(int port) : _server_fd(-1), _PORT(port), _addrlen(sizeof(_address)), _hello("HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>")
{
	this->initSocket();
}

HttpServer::~HttpServer()
{
	if (_server_fd != -1)
		close(this->_server_fd);
}

void HttpServer::initSocket()
{
	//Creazione socket file descriptot
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
		throw std::runtime_error("Cannot open socket");

	this->_address.sin_family = AF_INET; //IPv4
	this->_address.sin_addr.s_addr = INADDR_ANY; //significa che il server ascolterà su tutte le interfacce disponibili.
	this->_address.sin_port = htons(_PORT); //htons() converte il numero di porta in network byte order, necessario per garantire che il numero di porta sia interpretato correttamente su hardware con differenti ordinamenti dei byte.

	memset(_address.sin_zero, '\0', sizeof _address.sin_zero); //La funzione memset() è usata qui per riempire il resto della struttura con zeri. Questo campo è solo per padding e non ha un significato pratico; è necessario per mantenere la compatibilità con la struttura sockaddr più generale.

	// Associazione della socket all'indirizzo e alla porta specificati
	if (bind(this->_server_fd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("Cannot bind");
	}

	// Ascolto connesioni in entrata lasciando mettere in attesa 10 connesioni, prima di bloccare altre in attesa che si liberi spazio
	if (listen(this->_server_fd, 10) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("Cannot listen");
	}
}

void HttpServer::run()
{
	std::cout << "Server is running on port " << _PORT << std::endl;

	while (true)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(_server_fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket < 0)
		{
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        char buffer[30000] = {0};
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead < 0)
            std::cerr << "Error reading request" << std::endl;
        else if (bytesRead == 0)
            std::cout << "Client closed the connection" << std::endl;
        else
		{
            std::cout << "----- Request -----\n" << buffer << std::endl;
            write(clientSocket, _hello, strlen(_hello));
        }
        close(clientSocket);
    }
}




/* ### 1. `socket()`

La funzione `socket()` crea una nuova socket, ovvero un endpoint di comunicazione. Restituisce un file descriptor (un intero) che rappresenta la socket appena creata.

- **Sintassi**: `int socket(int domain, int type, int protocol)`
- **Parametri**:
  - `domain`: Specifica il dominio di comunicazione; `AF_INET` è usato per IPv4.
  - `type`: Specifica il tipo di socket; `SOCK_STREAM` per stream socket (TCP) e `SOCK_DGRAM` per datagram socket (UDP).
  - `protocol`: Specifica il protocollo; normalmente impostato a 0 per scegliere automaticamente il protocollo appropriato per il tipo dato.

### 2. `bind()`

La funzione `bind()` associa un indirizzo locale, come l'indirizzo IP e il numero di porta, alla socket. Questo è necessario per i server affinché i client possano connettersi a essi usando questo indirizzo.

- **Sintassi**: `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`
- **Parametri**:
  - `sockfd`: Il file descriptor della socket.
  - `addr`: Puntatore a una struttura `sockaddr` che contiene l'indirizzo IP e il numero di porta.
  - `addrlen`: La dimensione in byte della struttura puntata da `addr`.

### 3. `listen()`

Dopo il binding, la socket viene messa in ascolto per le connessioni in entrata. `listen()` marca la socket come passiva, pronta ad accettare connessioni in entrata.

- **Sintassi**: `int listen(int sockfd, int backlog)`
- **Parametri**:
  - `sockfd`: Il file descriptor della socket.
  - `backlog`: Il numero massimo di connessioni pendenti nella coda di ascolto.

### 4. `accept()`

La funzione `accept()` estrae la prima richiesta di connessione nella coda di ascolto per la socket in ascolto, crea una nuova socket connessa, e restituisce un nuovo file descriptor che si riferisce a quella socket. Questa nuova socket è quella usata per comunicare con il client.

- **Sintassi**: `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`
- **Parametri**:
  - `sockfd`: Il file descriptor della socket in ascolto.
  - `addr`: Puntatore a una struttura `sockaddr` che riceve l'indirizzo del client in connessione.
  - `addrlen`: Puntatore a una variabile che contiene la dimensione di `addr`; alla fine contiene la dimensione effettiva dell'indirizzo del client.

### 5. `read()`

La funzione `read()` legge i dati dalla socket nel buffer fornito.

- **Sintassi**: `ssize_t read(int fd, void *buf, size_t count)`
- **Parametri**:
  - `fd`: Il file descriptor da cui leggere (in questo caso, il file descriptor della socket connessa).
  - `buf`: Il buffer dove memorizzare i dati letti.
  - `count`: Il numero massimo di byte da leggere.
 */