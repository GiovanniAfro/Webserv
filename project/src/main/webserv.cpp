#include "webserv.h"

Webserver *Webserver::instance = NULL;

Webserver::Webserver(ConfigFile *configfile, Http *httpDirective)
	: _configfile(configfile), _httpDirective(httpDirective) {
	instance = this;
	signal(SIGINT, sigintHandler);
}

Webserver::~Webserver() {
	if (_configfile)
		delete _configfile;
	if (_httpDirective)
		delete _httpDirective;
}

Http *Webserver::getHttpDirective() {
	return _httpDirective;
}

void Webserver::sigintHandler(int signum) {
	if (signum == SIGINT)
	{
		if (Webserver::instance)
			delete Webserver::instance;
		exit(0);
	}
}
