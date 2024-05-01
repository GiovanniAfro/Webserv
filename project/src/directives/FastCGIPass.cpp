#include "FastCGIPass.hpp"

FastCGIPass::FastCGIPass()
: ADirective("fastcgi_pass", LOCATION_CONTEXT)
{}

FastCGIPass::FastCGIPass(const std::string& content)
: ADirective("fastcgi_pass", LOCATION_CONTEXT)
{
	std::stringstream	iss(content);
	std::string			token;
	unsigned int		tokenCount = 0;

	_isEnabled = false;
	while (iss >> token)
		++tokenCount;
	if (tokenCount == 1)
	{
		if (token != "self")
			throw std::runtime_error("fastcgi_pass : invalid content");
		else
			_isEnabled = true;
	}
	else if (tokenCount > 1)
		throw std::runtime_error("fastcgi_pass : invalid content");
}

FastCGIPass::FastCGIPass(const FastCGIPass &copy)
: ADirective(copy)
{ *this = copy; }

FastCGIPass::~FastCGIPass() {}

FastCGIPass &FastCGIPass::operator=(const FastCGIPass &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_isEnabled = other._isEnabled;

	return *this;
}

ADirective *FastCGIPass::clone() const {
	return new FastCGIPass(*this);
}

bool	FastCGIPass::getMode() const {
	return _isEnabled;
}
