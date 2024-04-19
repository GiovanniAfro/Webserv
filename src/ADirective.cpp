#include "ADirective.hpp"

ADirective::ADirective() {}

ADirective::ADirective(const std::string& type, int context)
: _type(type), _context(static_cast<uint16_t>(context))
{}

ADirective::ADirective(const ADirective& copy)
{ *this = copy; }

ADirective::~ADirective()
{
	// this->_blocks.pop_front();
	// std::cout << "PostPop : " << this->_blocks.size() << std::endl;

	for (std::vector<ADirective*>::iterator it = _blocks.begin(); it != _blocks.end(); ++it)
	{
		delete (*it);
	}
	for (std::map<std::string, ADirective*>::iterator it = _directives.begin(); it != _directives.end(); ++it)
	{
		delete it->second;
	}
	_directives.clear();
}

ADirective&	ADirective::operator=(const ADirective& other)
{
	if (this == &other)
		return *this;

	this->_type = other._type;
	this->_blocks = other._blocks;
	this->_context = other._context;
	this->_directives = other._directives;

	return *this;
}

const std::string&	ADirective::getType() const
{ return _type; }

std::vector<ADirective*>&	ADirective::getBlocks()
{return _blocks; }

size_t	ADirective::getBlocksSize() const
{ return _blocks.size(); }

void	ADirective::addBlock(ADirective* block)
{ _blocks.push_back(block); }

void	ADirective::addDirective(ADirective* directive)
{
	if (_directives.find(directive->getType()) == _directives.end())
	{
		ADirective*	newDirective = directive->clone();

		_directives[directive->getType()] = newDirective;
		_directives[directive->getType()]->addBlock(newDirective->clone());
	}
	else
	{
		_directives[directive->getType()]->addBlock(directive->clone());	// clone in addBlock?
	}
}

std::map<std::string, ADirective*>&	ADirective::getDirectives()
{ return _directives; }
