#include "WebServer.hpp"

int	main(int ac, char **av)
{
	WebServer	webServer;
	ConfigFile	file(&webServer);
	// (void)av;
	if (ac < 1 || ac > 2)
	{
		Log::error("Usage: ./webserv [CONFIG-FILE]");
		return 1;
	}

	if (ac == 2)
		file.setFilePath(av[1]);
	webServer.setConfigFile(file);

	file.parseConfigFile();
	webServer.startServers();

	// // Log::debug("PROVE");

	// for (std::vector<ADirective*>::iterator itServer = webServer.getServers().begin();itServer != webServer.getServers().end(); ++itServer)
	// {
	// 	// Log::debug("SERVER");
	// 	Listen*	listenDir = static_cast<Listen*>((*itServer)->getDirectives()["listen"]);


	// 	for (std::vector<ADirective*>::iterator itDir = listenDir->getBlocks().begin(); itDir != listenDir->getBlocks().end(); ++itDir)
	// 	{
	// 		// Log::debug("LISTEN");
	// 		Listen*	listen = static_cast<Listen*>((*itDir));
	// 		// std::cout <<  << std::endl;

	// 		for (std::set<uint16_t>::iterator itPort = listen->getPorts().begin(); itPort != listen->getPorts().end(); ++itPort)
	// 		{
	// 			// Log::debug("PORT");
	// 			// std::cout << *itPort << std::endl;
	// 		}

	// 	}

	// }

	return 0;
}
