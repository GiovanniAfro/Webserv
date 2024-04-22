/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:48:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:48:36 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

int	main(int ac, char **av) {
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
