/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcavanna <gcavanna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:48:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/03 12:56:08 by gcavanna         ###   ########.fr       */
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

	file.parseConfigFile();
	webServer.startServers();

	return 0;
}
