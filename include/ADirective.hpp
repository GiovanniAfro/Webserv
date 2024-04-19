#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <stdint.h>

// using std::cout;
// using std::cerr;
// using std::endl;
// using std::std::string;
// using std::ifstream;
// using std::vector;
// using std::std::map;
// using std::getline;
// using std::streampos;
// using std::exception;

#define GLOBAL_CONTEXT 0
#define HTTP_CONTEXT 1
#define SERVER_CONTEXT 2
#define LOCATION_CONTEXT 3

#define NUM_DIRECTIVES 7
#define HTTP_DIRECTIVE 0
#define INCLUDE_DIRECTIVE 1
#define SERVER_DIRECTIVE 2
#define LISTEN_DIRECTIVE 3
#define ROOT_DIRECTIVE 4
#define SERVER_NAME_DIRECTIVE 5
#define INDEX_DIRECTIVE 6

class	ADirective
{
	protected:
		// typedef	vector<ADirective*>::iterator	DirIt;

		std::string					_type;
		uint16_t					_context;
		std::vector<ADirective*>	_blocks;

		std::map<std::string, ADirective*>	_directives;

	public:
		ADirective();
		ADirective(const std::string& type, int context);
		ADirective(const ADirective& copy);
		virtual ~ADirective();
		ADirective&	operator=(const ADirective& other);
		virtual ADirective*		clone() const = 0;

		const std::string&				getType() const;
		std::vector<ADirective*>&		getBlocks();
		std::map<std::string, ADirective*>&	getDirectives();
		size_t						getBlocksSize() const;

		void	addBlock(ADirective* block);
		void	addDirective(ADirective* directive);
		void	addDirectiveRouter(const std::string& type);

};
