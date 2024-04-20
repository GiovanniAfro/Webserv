#pragma once

#include "webserv.h"
// #include "WebServer.hpp"

class	WebServer;

class	ConfigFile
{
	private:
		const char			*_filePath;
		WebServer*			_webServer;
		uint16_t			_context;

		ConfigFile();

	public:
		ConfigFile(WebServer* server);
		ConfigFile(const char *filePath, WebServer* server);
		~ConfigFile();

		// const std::string&	getPath() const;
		void	setFilePath(const char *filePath);
		// WebServer&			getServer();
		// uint16_t			getContext() const;

		int	parseConfigFile();
		int	parserRouter(std::ifstream& inputFile, const std::string& header, std::string& content, uint16_t context);
		int	parseHttp(std::ifstream& inputFile, std::string& line);
		int	parseInclude();
		int	parseServers(std::ifstream& inputFile, std::string& line);
		int	parseListen(const std::string& content);
		int	parseRoot(const std::string& content, uint16_t context);
		int	parseServerName(const std::string& content);
		int	parseErrorPage(const std::string& content);

};
