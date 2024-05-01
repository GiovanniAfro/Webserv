#pragma once

#include "webserv.h"

class FastCGIPass : public ADirective
{
	private:
		bool _isEnabled;

		FastCGIPass();
	public:
		FastCGIPass(const std::string& content);
		FastCGIPass(const FastCGIPass& copy);
		~FastCGIPass();
		FastCGIPass&	operator=(const FastCGIPass& other);

		ADirective*	clone() const;

		bool	getMode() const;
};
