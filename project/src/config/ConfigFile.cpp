#include "ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(WebServer* server)
: _filePath("etc/webserv/webserv.conf")
{
	std::ifstream	inputFile;

	inputFile.open(this->_filePath);
	if (!inputFile.is_open())
	{
		throw std::runtime_error("webserv: ConfigFile: file does not exist");
	}
	inputFile.close();
	_webServer = server;
	_context = GLOBAL_CONTEXT;
}

ConfigFile::ConfigFile(const char *filePath, WebServer* server)
: _filePath(filePath)
{
	std::ifstream	inputFile;

	inputFile.open(this->_filePath);
	if (!inputFile.is_open())
	{
		throw std::runtime_error("webserv: ConfigFile: file does not exist");
	}
	inputFile.close();
	_webServer = server;
	_context = GLOBAL_CONTEXT;
}

void	ConfigFile::setFilePath(const char *filePath)
{
	this->_filePath = filePath;
}

ConfigFile::~ConfigFile() {
}

int	ConfigFile::parseConfigFile()
{
	std::ifstream		inputFile;
	std::string			line, header, content;
	std::stringstream	streamBlock;

	Log::debug("parseConfigFile");
	inputFile.open(this->_filePath);
	if (!inputFile.is_open())
	{
		// Log::error("webserv: ConfigFile: file does not exist");
		return -1;
	}
	while (getline(inputFile, line))
	{
		std::cout << "line : " << line << std::endl;
		line = strip(line);
		if (line.empty() || isComment(line))
			continue;
		header = firstToken(line);
		if (header.empty())
			return -1;
		std::cout << "header : " << header << std::endl;
		content = secondToken(line);
		std::cout << "content : " << content << std::endl;
		if (header == "http")
		{
			this->_context = HTTP_CONTEXT;
			this->parserRouter(inputFile, header, content, GLOBAL_CONTEXT);

		}
		//	check '{}'
	}

	this->parserRouter(inputFile, "include", line, HTTP_CONTEXT);

	Log::debug("PROVE");
	Http*		con = static_cast<Http*>(this->_webServer->getConfigs()[0]);
	std::cout << "Http : " << this->_webServer->getConfigs().size() << std::endl;
	Include*	inc = static_cast<Include*>(con->getDirectives()["include"]);
	std::cout << "Include size : " << inc->getBlocks().size() << std::endl;
	for (std::vector<ADirective*>::iterator it = inc->getBlocks().begin(); it != inc->getBlocks().end(); ++it)
	{
		std::cout << static_cast<Include*>(*it)->getPath() << std::endl;
	}

	// size_t	serSize = this->_webServer->getServers().size();
	// std::cout << "Server : " << serSize << std::endl;
	for (std::vector<ADirective*>::iterator it = this->_webServer->getServers().begin(); it != this->_webServer->getServers().end(); ++it)
	{
		Server*	ser = static_cast<Server*>(*it);
		Listen*	lisBlock = static_cast<Listen*>(ser->getDirectives()["listen"]);

		std::cout << "Listen : " << lisBlock->getBlocksSize() << std::endl;
		for (std::vector<ADirective*>::iterator it = lisBlock->getBlocks().begin(); it != lisBlock->getBlocks().end(); ++it)
		{
			std::cout << std::endl;
			Listen* 			lis = static_cast<Listen*>(*it);
			std::set<uint16_t>	ports = lis->getPorts();
			std::cout << "ports : " << lis->getPorts().size() << std::endl;
			for (std::set<uint16_t>::iterator it = ports.begin(); it != ports.end(); ++it)
			{
				std::cout << *it << std::endl;
			}
			std::cout << std::endl;
		}
	}


	return 0;
}

int	ConfigFile::parseHttp(std::ifstream& inputFile, std::string& line)
{
	std::string	header, content;

	Log::debug("parseHttp");

	this->_webServer->addConfig();
	while (getline(inputFile, line))
	{
	std::cout << "line : " << line << std::endl;
		line = strip(line);
		if (line.empty() || isComment(line) /* || isBracket(line) */)
			continue;
		else if (isClosedBracket(line))
			break;
		header = firstToken(line);
	std::cout << "header : " << header << std::endl;
		if (header.empty())
			return -1;
		content = secondToken(line);
	std::cout << "content : " << content << std::endl;
		if (header == "include")
		{
			glob_t	globPaths;
			glob(content.c_str(), GLOB_TILDE, NULL, &globPaths);
			for (unsigned int i = 0; i < globPaths.gl_pathc; ++i)
			{
				std::cout << globPaths.gl_pathv[i] << std::endl;
				Include	include(globPaths.gl_pathv[i]);
				this->_webServer->getConfigs().back()->addDirective(&include);	// getConfigs()[0]
			}
			globfree(&globPaths);
		}
		else
		{
			this->parserRouter(inputFile, header, content, HTTP_CONTEXT);
		}
	}
	// this->_webServer->addConfig(&http);	// Http loses include blocks ???

	return 0;
}

int	ConfigFile::parseInclude()
{
	Include*	include = NULL;

	Log::debug("parseInclude");
	// for (std::vector<ADirective*>::iterator it = _webServer->getConfigs().begin(); it != _webServer->getConfigs().end(); ++it)
	// {}
	if (_webServer->getConfigs()[0]->getDirectives().find("include") !=
		_webServer->getConfigs()[0]->getDirectives().end())
	{
		include = static_cast<Include*>
				(_webServer->getConfigs()[0]->getDirectives()["include"]);
	}
	else
	{
		return -1;
	}

	for (std::vector<ADirective*>::iterator it = include->getBlocks().begin(); it != include->getBlocks().end(); ++it)
	{
		Include*		include = static_cast<Include*>(*it);
		std::ifstream	inputFile;
		std::string		line, header, content;
		std::vector<ADirective*>	includeBlocks(include->getBlocks());

		// includeBlocks.pop

		std::cout << include->getPath() << std::endl;

		inputFile.open((include->getPath()).c_str());
		if (!inputFile.is_open())
		{
			std::cerr << "webserv: ConfigFile: file does not exist" << std::endl;
			return -1;
		}
		while (getline(inputFile, line))
		{
		std::cout << "line : " << line << std::endl;
			line = strip(line);
			if (line.empty() || isComment(line) || isBracket(line))
				continue;
			header = firstToken(line);
		std::cout << "header : " << header << std::endl;
			if (header.empty())
				return -1;
			content = secondToken(line);
		std::cout << "content : " << content << std::endl;
			if (header == "server")
			{
				this->parserRouter(inputFile, header, content, HTTP_CONTEXT);
			}
			else
			{
				this->parserRouter(inputFile, header, content, HTTP_CONTEXT);
			}
		}
	}

	return 0;
}

int	ConfigFile::parseServers(std::ifstream& inputFile, std::string& line)
{
	std::string	header, content;

	Log::debug("parseServers");

	this->_webServer->addServer();
	while (getline(inputFile, line))
	{
	std::cout << "line : " << line << std::endl;
		line = strip(line);
		if (line.empty() || isComment(line)/*  || isBracket(line) */)	// isOpenBracket() enough?
			continue;
		else if (isClosedBracket(line))
			break;
		header = firstToken(line);
	std::cout << "header : " << header << std::endl;
		if (header.empty())
			return -1;
		content = secondToken(line);
	std::cout << "content : " << content << std::endl;
		if (isServerDirective(header))
		{
			this->parserRouter(inputFile, header, content, SERVER_CONTEXT);
		}
	}

	return 0;
}

int	ConfigFile::parserRouter(std::ifstream& inputFile, const std::string& header, std::string& content, uint16_t context)	// inputFile pointer so that it can be passed as NULL?
{
	Log::debug("parserRouter");

	int	index = whichDirective(header);

	std::cout << index << std::endl;

	if (index < 0)
	{
		// Log::error("parserRouter : invalid header");
		return -1;
	}
	if (!checkContext(index, context))
	{
		// Log::error("parserRouter : invalid context");
		return -1;
	}
	switch (index)
	{
		case HTTP_DIRECTIVE:
			return this->parseHttp(inputFile, content);

		case INCLUDE_DIRECTIVE:
			return this->parseInclude();

		case SERVER_DIRECTIVE:
			return this->parseServers(inputFile, content);

		case LISTEN_DIRECTIVE:
			return this->parseListen(content);

		case ROOT_DIRECTIVE:
			return this->parseRoot(content, context);

		case SERVER_NAME_DIRECTIVE:

			break;

		case INDEX_DIRECTIVE:

			break;

		case ERRORPAGE_DIRECTIVE:
			return this->parseErrorPage(content);

		default:
			break;
	}

	return 0;
}

int	ConfigFile::parseListen(const std::string& content)
{
	Server*				server = static_cast<Server*>(this->_webServer->getServers().back());
	Listen				directive;
	std::stringstream	iss(content);
	std::string			token;

	Log::debug("parseListen");

	server->addDirective(&directive);
	Listen*	listen = static_cast<Listen*>(server->getDirectives()["listen"]->getBlocks().back());
	while (iss >> token)
	{
		std::cout << token << std::endl;

		bool	isPort = true;
		for (std::string::iterator it = token.begin(); it != token.end(); ++it)
		{
			if (!::isdigit(*it))
				isPort = false;
		}
		if (isPort)
		{
			int	port = atoi(token.c_str());
			if (port < 0 || port > 65535)
			{
				std::cerr << "listen : invalid port" << std::endl;
				return -1;
			}
			listen->addPort(static_cast<uint16_t>(port));

		}
		else if (std::count(token.begin(), token.end(), ':') == 1)	// addressPort
		{
			std::string	address = token.substr(0, token.find(":")).c_str();
			int			port = atoi(token.substr(token.find(":") + 1).c_str());

			std::cout << address << std::endl;
			std::cout << port << std::endl;

			if (port < 0 || port > 65535)
			{
				// Log::error("listen : invalid port");
				return -1;
			}
			// struct in_addr	addr;	Check "localhost" first?
			// if (inet_pton(AF_INET, address.c_str(), &addr) == 1)
			// {
			// 	// valid IPv4
			// }
			// else
			// {
			// 	std::cerr << "listen : invalid ip address" << std::endl;
			// 	return -1;
			// }
			listen->addPort(static_cast<uint16_t>(port));
			listen->addAddress(address);
			listen->addAddressPort(address, static_cast<uint16_t>(port));
		}
		else	// address or default_server
		{
			struct in_addr	addr;

			if (token == "default_server")
			{
				listen->setDefaultServer();
			}
			else if (inet_pton(AF_INET, token.c_str(), &addr) == 1)
			{
				// valid IPv4
				listen->addAddress(token);

			}
			else
			{
				std::cerr << "listen : invalid ip address" << std::endl;
				return -1;
			}
		}

	}

	return 0;
}

int	ConfigFile::parseRoot(const std::string& content, uint16_t context)
{
	ADirective*	server = this->_webServer->getServers().back();

	try
	{
		Root	directive(context, content);
		server->addDirective(&directive);
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseServerName(const std::string& content)
{
	ADirective*	server = this->_webServer->getServers().back();

	try
	{
		ServerName	directive(content);
		server->addDirective(&directive);
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseErrorPage(const std::string& content)
{
	// ADirective*				server = this->_webServer->getServers().back();
	// ErrorPage				directive;
	std::vector<enum HTTP_STATUS>	codes;
	int								response = 0;
	std::string						uri;
	std::stringstream				iss(content);
	std::string						token;

	Log::debug("parseErrorPage");

	while (iss >> token)
	{
		std::cout << token << std::endl;

		bool	isCode = true;
		for (std::string::iterator it = token.begin() + 1; it != token.end(); ++it)
			if (!::isdigit(*it))
				isCode = false;
		if ((!isCode && codes.empty()) || 
			(!::isdigit(token.front()) && codes.empty()))
		{
			Log::error("error_page : invalid content");
			// return Log::error("error_page : invalid content");
		}

		if (::isdigit(token.front()))
		{
			int	code = (atoi(token.c_str()));
			isCode = false;
			for (std::vector<HTTP_STATUS>::iterator it = allHttpStatus.begin(); it != allHttpStatus.end(); ++it)
			{
				if (code == *it)
				{
					isCode = true;
					codes.push_back(static_cast<HTTP_STATUS>(code));
					Log::debug("code ok");
					break;
				}
			}
			if (!isCode)
			{
				Log::error("error_page : invalid code");
				// return Log::error("error_page : invalid code");
			}
			continue;
		}

		switch(token.front())
		{
			case '=' :
				if (response || codes.empty() || !isCode)
				{
					Log::error("error_page : invalid content");
					// return Log::error("error_page : invalid content");
				}
				isCode = false;
				response = (atoi(token.c_str() + 1));
				for (std::vector<HTTP_STATUS>::iterator it = allHttpStatus.begin(); it != allHttpStatus.end(); ++it)
				{
					if (response == *it)
					{
						isCode = true;
						response = static_cast<HTTP_STATUS>(response);
						Log::debug("response ok");
					}					
				}
				if (!isCode)
				{
					Log::error("error_page : invalid response code");
					// return Log::error("error_page : invalid response code");
				}
				break;

			case '/' :	// isLimitModifier() ?
				if (codes.empty() || !uri.empty())
				{
					Log::error("error_page : missing code");
					// return Log::error("error_page : missing code");
				}
				uri = token;
				Log::debug("uri ok");
				Log::debug(token);
				break;

			default :
				Log::error("error_page : invalid content");
				// return Log::error("error_page : invalid content");
		}
	}

	Log::info("codes:");
	std::cout << codes.size() << std::endl;
	for (std::vector<HTTP_STATUS>::iterator it = codes.begin(); it != codes.end(); ++it)
		std::cout << (*it) << std::endl;
	Log::info("response");
	std::stringstream	ss;
	ss << response;
	Log::info(ss.str());
	Log::info("uri");
	Log::info(uri);

	return 0;
}
