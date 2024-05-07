/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:48:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/06 16:44:36 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

int	main(int ac, char **av) {
	signal(SIGINT, WebServer::sigintHandler);
	WebServer	webServer;
	ConfigFile	file(&webServer);

	if (ac < 1 || ac > 2)
		return Log::error("Usage: ./webserv [CONFIG-FILE]");

	if (ac == 2)
		file.setFilePath(av[1]);
	webServer.setConfigFile(file);

	if (file.parseConfigFile() == -1)
		return Log::error("WebServer : parsing failed");
	webServer.startServers();

	return 0;
}
