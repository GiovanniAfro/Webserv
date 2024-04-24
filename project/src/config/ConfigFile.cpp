/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:38:32 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 21:27:06 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(WebServer *server)
: _filePath("etc/webserv/webserv.conf")
{
	std::ifstream	inputFile;

	inputFile.open(this->_filePath);
	if (!inputFile.is_open())
		throw std::runtime_error("webserv: ConfigFile: file does not exist");
	inputFile.close();
	_webServer = server;
	_context = GLOBAL_CONTEXT;
}

ConfigFile::ConfigFile(const char *filePath, WebServer *server)
: _filePath(filePath)
{
	std::ifstream	inputFile;

	inputFile.open(this->_filePath);
	if (!inputFile.is_open()) {
		throw std::runtime_error("webserv: ConfigFile: file does not exist");
	}
	inputFile.close();
	_webServer = server;
	_context = GLOBAL_CONTEXT;
}

void	ConfigFile::setFilePath(const char *filePath)
{ this->_filePath = filePath; }

ConfigFile::~ConfigFile() {}

int	ConfigFile::parseConfigFile()
{
	std::ifstream		inputFile;
	std::string			line, header, content;
	std::stringstream	streamBlock;

	Log::debug("parseConfigFile");
	inputFile.open(this->_filePath);
	if (!inputFile.is_open()) {
		// return Log::error("webserv: ConfigFile: file does not exist");
		return -1;
	}
	while (getline(inputFile, line)) {
		std::cout << "line : " << line << std::endl;
		line = strip(line);
		if (line.empty() || isComment(line))
			continue;
		header = firstToken(line);
		if (header.empty())
			return Log::error("ConfigFile : parseConfigFile : invalid header");
		std::cout << "header : " << header << std::endl;
		content = secondToken(line);
		std::cout << "content : " << content << std::endl;
		if (header == "http") {
			this->_context = HTTP_CONTEXT;
			this->parserRouter(inputFile, header, content, GLOBAL_CONTEXT);

		}
		//	check '{}'
	}

	this->parserRouter(inputFile, "include", line, HTTP_CONTEXT);

	Log::debug("PROVE");
	Http *con = static_cast<Http *>(this->_webServer->getConfigs()[0]);
	std::cout << "Http : " << this->_webServer->getConfigs().size() << std::endl;
	Include *inc = static_cast<Include *>(con->getDirectives()["include"]);
	std::cout << "Include size : " << inc->getBlocks().size() << std::endl;
	for (std::vector<ADirective *>::iterator it = inc->getBlocks().begin(); it != inc->getBlocks().end(); ++it) {
		std::cout << static_cast<Include *>(*it)->getPath() << std::endl;
	}

	Log::debug("SERVERS");
	for (std::vector<ADirective *>::iterator it = this->_webServer->getServers().begin(); it != this->_webServer->getServers().end(); ++it)
	{
		Server *ser = static_cast<Server *>(*it);
		Listen *lisBlock = static_cast<Listen *>(ser->getDirectives()["listen"]);

		Log::debug("LISTENS");
		std::cout << "Listen : " << lisBlock->getBlocksSize() << std::endl;
		for (std::vector<ADirective *>::iterator it = lisBlock->getBlocks().begin(); it != lisBlock->getBlocks().end(); ++it)
		{
			std::cout << std::endl;
			Listen *lis = static_cast<Listen *>(*it);
			std::set<uint16_t>	ports = lis->getPorts();
			std::cout << "ports : " << lis->getPorts().size() << std::endl;
			for (std::set<uint16_t>::iterator it = ports.begin(); it != ports.end(); ++it) {
				std::cout << *it << std::endl;
			}
			std::cout << std::endl;
		}

		Log::debug("INDEX");
		if (ser->getDirectives().find("index") != ser->getDirectives().end())
		{
			Index*	indBlock = static_cast<Index*>(ser->getDirectives()["index"]);
			for (std::vector<std::string>::iterator itInd = indBlock->getFiles().begin(); itInd != indBlock->getFiles().end(); ++itInd)
				std::cout << *itInd << std::endl;
		}
		Log::debug("ERROR_PAGE");
		if (ser->getDirectives().find("error_page") != ser->getDirectives().end())
		{
			for (std::vector<ADirective*>::iterator it = ser->getDirectives()["error_page"]->getBlocks().begin(); it != ser->getDirectives()["error_page"]->getBlocks().end(); ++it)
			{
				ErrorPage*	errBlock = static_cast<ErrorPage*>(*it);
				std::cout << "error_page codes : " << errBlock->getCodes().size() << std::endl;
				std::cout << "error_page response : " << errBlock->getResponse() << std::endl;
				std::cout << "error_page uri : " << errBlock->getUri() << std::endl;
				for (std::vector<enum HTTP_STATUS>::iterator itErr = errBlock->getCodes().begin(); itErr != errBlock->getCodes().end(); ++itErr)
					std::cout << "error_page code : " << *itErr << std::endl;
			}
		}
		Log::debug("AUTOINDEX");
		if (ser->getDirectives().find("autoindex") != ser->getDirectives().end())
		{
			Autoindex*	autoBlock = static_cast<Autoindex*>(ser->getDirectives()["autoindex"]);
			std::cout << "autoindex : " << (autoBlock->getMode()? "on" : "off") << std::endl;
		}
		Log::debug("CLIENT_MAX_BODY_SIZE");
		if (ser->getDirectives().find("client_max_body_size") != ser->getDirectives().end())
		{
			ClientMaxBodySize* clieBlock = static_cast<ClientMaxBodySize*>(ser->getDirectives()["client_max_body_size"]);
			std::cout << "client_max_body_size : " << clieBlock->getSize() << std::endl;
		}
		Log::debug("LOCATIONS");
		if (ser->getDirectives().find("location") != ser->getDirectives().end())
		{
			Location*	locBlock = static_cast<Location*>(ser->getDirectives()["location"]->getBlocks().back());
			std::cout << "location uri : " << locBlock->getUri() << std::endl;
			if (locBlock->getDirectives().find("limit_except") != locBlock->getDirectives().end())
			{
				LimitExcept* limBlock = static_cast<LimitExcept*>(locBlock->getDirectives()["limit_except"]);
				std::cout << "limit_except : " << limBlock->getMethod() << std::endl;
			}
			if (locBlock->getDirectives().find("index") != locBlock->getDirectives().end())
			{
				Index* indBlock = static_cast<Index*>(locBlock->getDirectives()["index"]);
				std::cout << "index : " << indBlock->getLastElement() << std::endl;
				for (std::vector<std::string>::iterator itInd = indBlock->getFiles().begin(); itInd != indBlock->getFiles().end(); ++itInd)
					std::cout << *itInd << std::endl;
			}
			if (locBlock->getDirectives().find("root") != locBlock->getDirectives().end())
			{
				Root* rooBlock = static_cast<Root*>(locBlock->getDirectives()["root"]);
				std::cout << "root : " << rooBlock->getPath() << std::endl;
			}
			if (locBlock->getDirectives().find("client_max_body_size") != locBlock->getDirectives().end())
			{
				ClientMaxBodySize* clieBlock = static_cast<ClientMaxBodySize*>(locBlock->getDirectives()["client_max_body_size"]);
				std::cout << "client_max_body_size : " << clieBlock->getSize() << std::endl;
			}
		}
	}


	return 0;
}

int	ConfigFile::parseHttp(std::ifstream &inputFile, std::string &line) {
	std::string	header, content;

	Log::debug("parseHttp");

	this->_webServer->addConfig();
	while (getline(inputFile, line)) {
		std::cout << "line : " << line << std::endl;
		line = strip(line);
		if (line.empty() || isComment(line) /* || isBracket(line) */)
			continue;
		else if (isClosedBracket(line))
			break;
		header = firstToken(line);
		std::cout << "header : " << header << std::endl;
		if (header.empty())
			return Log::error("ConfigFile : parseHttp : invalid header");
		content = secondToken(line);
		std::cout << "content : " << content << std::endl;
		if (header == "include") {
			glob_t	globPaths;
			glob(content.c_str(), GLOB_TILDE, NULL, &globPaths);
			for (unsigned int i = 0; i < globPaths.gl_pathc; ++i) {
				std::cout << globPaths.gl_pathv[i] << std::endl;
				Include	include(globPaths.gl_pathv[i]);
				this->_webServer->getConfigs().back()->addDirective(&include);	// getConfigs()[0]
			}
			globfree(&globPaths);
		}
		else {
			this->parserRouter(inputFile, header, content, HTTP_CONTEXT);
		}
	}
	// this->_webServer->addConfig(&http);	// Http loses include blocks ???

	return 0;
}

int	ConfigFile::parseInclude() {
	Include *include = NULL;

	Log::debug("parseInclude");
	// for (std::vector<ADirective*>::iterator it = _webServer->getConfigs().begin(); it != _webServer->getConfigs().end(); ++it)
	// {}
	if (_webServer->getConfigs()[0]->getDirectives().find("include") !=
		_webServer->getConfigs()[0]->getDirectives().end()) {
		include = static_cast<Include *>
			(_webServer->getConfigs()[0]->getDirectives()["include"]);
	}
	else {
		return -1;
	}

	for (std::vector<ADirective *>::iterator it = include->getBlocks().begin(); it != include->getBlocks().end(); ++it) {
		Include *include = static_cast<Include *>(*it);
		std::ifstream	inputFile;
		std::string		line, header, content;
		std::vector<ADirective *>	includeBlocks(include->getBlocks());

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
				return Log::error("ConfigFile : parseInclude : invalid header");
			content = secondToken(line);
			std::cout << "content : " << content << std::endl;
			this->parserRouter(inputFile, header, content, HTTP_CONTEXT);
		}
	}

	return 0;
}

int	ConfigFile::parseServers(std::ifstream &inputFile, std::string &line) {
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
			return Log::error("ConfigFile : parseServers : invalid header");
		content = secondToken(line);
		std::cout << "content : " << content << std::endl;
		// if (isServerDirective(header))
			this->parserRouter(inputFile, header, content, SERVER_CONTEXT);
	}

	return 0;
}

int	ConfigFile::parserRouter(std::ifstream &inputFile, const std::string &header, std::string &content, uint16_t context)	// inputFile pointer so that it can be passed as NULL?
{
	Log::debug("parserRouter");
	std::cout << "context : " << context << std::endl;
	int	index = whichDirective(header);

	std::cout << index << std::endl;

	if (index < 0)
		return Log::error("parserRouter : invalid header");
	if (!checkContext(index, context))
		return Log::error("parserRouter : invalid context");
	switch (index) {
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
			return this->parseServerName(content);
		case INDEX_DIRECTIVE:
			return this->parseIndex(content, context);
		case ERRORPAGE_DIRECTIVE:
			return this->parseErrorPage(content, context);
		case LOCATION_DIRECTIVE:
			return this->parseLocation(content, context, inputFile);
		case AUTOINDEX_DIRECTIVE:
			return this->parseAutoIndex(content, context);
		case LIMITEXCEPT_DIRECTIVE:
			return this->parseLimitExcept(content);
		case CLIENTMAXBODYSIZE_DIRECTIVE:
			return this->parseClientMaxBodySize(content, context);
		default:
			break;
	}

	return 0;
}

int	ConfigFile::parseListen(const std::string &content)
{
	std::pair< std::string, std::set<uint16_t> >	addressPort;
	std::string										ipAddress;
	std::set<uint16_t>								ports;
	bool											isDefault = false;
	std::istringstream	iss(content);
	std::string			token;

	Log::debug("parseListen");

	while (iss >> token)
	{
		std::cout << token << std::endl;

		bool	isPort = true;
		for (std::string::iterator it = token.begin(); it != token.end(); ++it)
			if (!::isdigit(*it))
				isPort = false;

		if (isPort)
		{
			int	port = atoi(token.c_str());
			if (port < 0 || port > 65535)
				return Log::error("listen : invalid port");
			ports.insert(static_cast<uint16_t>(port));
		}
		else if (std::count(token.begin(), token.end(), ':') == 1)	// addressPort
		{
			if (!ipAddress.empty())
				return Log::error("listen : IP address is duplicated");

			std::string	address = token.substr(0, token.find(":")).c_str();
			int			port = atoi(token.substr(token.find(":") + 1).c_str());

			std::cout << address << std::endl;
			std::cout << port << std::endl;

			if (port < 0 || port > 65535)
				return Log::error("listen : invalid port");

			// struct in_addr	addr;	Check "localhost" first?
			// if (inet_pton(AF_INET, address.c_str(), &addr) == 1)
			// {
			// 	// valid IPv4
			// }
			// else
			// 	return Log::error("listen : invalid ip address");

			ipAddress = address;
			ports.insert(static_cast<uint16_t>(port));
		}
		else	// address or default_server
		{
			struct in_addr	addr;

			if (token == "default_server")
			{
				if (isDefault)
					return Log::error("listen : default_server directive is duplicated");
				isDefault = true;;
			}
			else if (inet_pton(AF_INET, token.c_str(), &addr) == 1)
			{
				// valid IPv4
				if (!ipAddress.empty())
					return Log::error("listen : IP address is duplicated");
				ipAddress = token;
			}
			else
				return Log::error("listen : invalid ip address");
		}
	}
	addressPort = std::make_pair(ipAddress, ports);

	try
	{
		Server *server = static_cast<Server *>(this->_webServer->getServers().back());
		Listen	directive(addressPort, ipAddress, ports, isDefault);
		server->addDirective(&directive);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseRoot(const std::string &content, uint16_t context) {

	Log::debug("parseRoot");

	try
	{
		ADirective*	contextDirective = NULL;

		if (context == HTTP_CONTEXT)
			contextDirective = this->_webServer->getConfigs()[0];
		else if (context == SERVER_CONTEXT)
			contextDirective = this->_webServer->getServers().back();
		else if (context == LOCATION_CONTEXT)
			contextDirective = this->_webServer->getServers().back()->getDirectives()["location"]->getBlocks().back();

		Root	directive(context, content);
		contextDirective->addDirective(&directive);
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseServerName(const std::string &content) {

	Log::debug("parseServerName");

	try
	{
		ADirective *server = this->_webServer->getServers().back();
		ServerName	directive(content);
		server->addDirective(&directive);
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseIndex(const std::string &content, uint16_t context)
{
	Log::debug("parseIndex");

	try
	{
		ADirective*	contextDirective = NULL;

		if (context == HTTP_CONTEXT)
			contextDirective = this->_webServer->getConfigs()[0];
		else if (context == SERVER_CONTEXT)
			contextDirective = this->_webServer->getServers().back();
		else if (context == LOCATION_CONTEXT)
			contextDirective = this->_webServer->getServers().back()->getDirectives()["location"]->getBlocks().back();

		Index	directive(content, context);
		contextDirective->addDirective(&directive);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseErrorPage(const std::string &content, uint16_t context)
{
	std::vector<enum HTTP_STATUS>	codes;
	int								response = 0;
	std::string						uri;
	std::stringstream				iss(content);
	std::string						token;

	Log::debug("parseErrorPage");

	while (iss >> token)
	{
		std::cout << token << std::endl;

		// if (token == "=")	// response = CGI response ?
		// {
		// 	if ((iss >> token) && token[0] == '/')
		// 		if (codes.empty() || !uri.empty())
		// 			return Log::error("error_page : missing code");
		// 	uri = token;
		// }

		bool	isCode = true;
		for (std::string::iterator it = token.begin() + 1; it != token.end(); ++it)
			if (!::isdigit(*it))
				isCode = false;
		if ((!isCode && codes.empty()) || (!::isdigit(token[0]) && codes.empty()))
			return Log::error("error_page : invalid content");

		if (::isdigit(token[0]))
		{
			int	code = (atoi(token.c_str()));
			isCode = false;
			for (std::vector<HTTP_STATUS>::iterator it = allHttpStatus.begin(); it != allHttpStatus.end(); ++it)
			{
				if (code == *it)
				{
					isCode = true;
					codes.push_back(static_cast<HTTP_STATUS>(code));
					break;
				}
			}
			if (!isCode)
				return Log::error("error_page : invalid code");
			continue;
		}

		switch (token[0])
		{
			case '=':
				if (response || codes.empty() || !isCode)
					return Log::error("error_page : invalid response content");
				isCode = false;
				response = (atoi(token.c_str() + 1));
				for (std::vector<HTTP_STATUS>::iterator it = allHttpStatus.begin(); it != allHttpStatus.end(); ++it) {
					if (response == *it)
					{
						isCode = true;
						response = static_cast<HTTP_STATUS>(response);
					}
				}
				if (!isCode)
					return Log::error("error_page : invalid response code");
				break;

			case '/':	// isLimitModifier() ?
				if (codes.empty() || !uri.empty())
					return Log::error("error_page : missing code");
				uri = token;
				break;

			default:
				return Log::error("error_page : invalid content");
		}
	}
	if (response == 0)
		response = static_cast<int>(codes.back());

	try
	{
		ADirective*	contextDirective = NULL;

		if (context == HTTP_CONTEXT)
			contextDirective = this->_webServer->getConfigs()[0];
		else if (context == SERVER_CONTEXT)
			contextDirective = this->_webServer->getServers().back();
		else if (context == LOCATION_CONTEXT)
			contextDirective = this->_webServer->getServers().back()->getDirectives()["location"]->getBlocks().back();

		ErrorPage	errorPage(context, codes, static_cast<HTTP_STATUS>(response), uri);
		contextDirective->addDirective(&errorPage);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseLocation(const std::string &content, uint16_t context, std::ifstream& inputFile)
{
	Log::debug("parseLocation");

	if (content.empty())
		return Log::error("location : empty content");

	std::stringstream	iss(content);
	std::string			token, modifier, uri;

	while (iss >> token)
	{
		if (isLocationModifier(token))
		{
			if (!modifier.empty())
				return Log::error("location : modifier is duplicated");
			modifier = token;
		}
		uri = token;
		break;
	}

	try
	{
		ADirective*	server = this->_webServer->getServers().back();
		Location	location(context, Location::_modifierToEnum(modifier), uri);
		server->addDirective(&location);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	std::string	line, header, directiveContent;

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
			return Log::error("ConfigFile : parseLocation : invalid header");
		directiveContent = secondToken(line);
		std::cout << "content : " << directiveContent << std::endl;
		this->parserRouter(inputFile, header, directiveContent, LOCATION_CONTEXT);
	}

	return 0;
}

int	ConfigFile::parseAutoIndex(const std::string &content, uint16_t context)
{
	Log::debug("parseAutoIndex");

	try
	{
		ADirective*	contextDirective = NULL;

		if (context == HTTP_CONTEXT)
			contextDirective = this->_webServer->getConfigs()[0];
		else if (context == SERVER_CONTEXT)
			contextDirective = this->_webServer->getServers().back();
		else if (context == LOCATION_CONTEXT)
			contextDirective = this->_webServer->getServers().back()->getDirectives()["location"]->getBlocks().back();

		Autoindex	autoindex(context, content);
		contextDirective->addDirective(&autoindex);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseLimitExcept(const std::string &content)
{
	Log::debug("parseLimitExcept");

	enum HTTP_METHOD	method = Http::_methodToEnum(content);

	if (method == UNKNOWN)
		return Log::error("limit_except : unknown method");

	try
	{
		ADirective	*location = this->_webServer->getServers().back()->getDirectives()["location"]->getBlocks().back();
		LimitExcept	directive(LOCATION_CONTEXT, method);
		location->addDirective(&directive);

	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}

int	ConfigFile::parseClientMaxBodySize(const std::string &content, uint16_t context)
{
	Log::debug("parseClientMaxBodySize");

	std::stringstream	iss(content);
	std::string			token;
	int					size = 0;

	while (iss >> token)
	{
		if (size)
			return Log::error("client_max_body_size : invalid content");
		long long	num = std::strtol(token.c_str(), NULL, 10);
		if ((num == LONG_MAX || num == LONG_MIN) && errno == ERANGE)
			return Log::error("client_max_body_size : invalid size");
		if (num < std::numeric_limits<int>::min() || num > std::numeric_limits<int>::max())
			return Log::error("client_max_body_size : invalid size");
		size = static_cast<size_t>(num);
	}

	try
	{
		ADirective*	contextDirective = NULL;

		if (context == HTTP_CONTEXT)
			contextDirective = this->_webServer->getConfigs()[0];
		else if (context == SERVER_CONTEXT)
			contextDirective = this->_webServer->getServers().back();
		else if (context == LOCATION_CONTEXT)
			contextDirective = this->_webServer->getServers().back()->getDirectives()["location"]->getBlocks().back();

		// std::cout << "clientmax context : " << 

		ClientMaxBodySize	clientMaxBodySize(context, size);
		contextDirective->addDirective(&clientMaxBodySize);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << '\n';
		return -1;
	}

	return 0;
}
