/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:31:07 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/25 17:05:31 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#define LIMITEXCEPT_CONTEXT 4

#define NUM_DIRECTIVES 15
#define HTTP_DIRECTIVE 0
#define INCLUDE_DIRECTIVE 1
#define SERVER_DIRECTIVE 2
#define LISTEN_DIRECTIVE 3
#define ROOT_DIRECTIVE 4
#define SERVER_NAME_DIRECTIVE 5
#define INDEX_DIRECTIVE 6
#define ERRORPAGE_DIRECTIVE 7
#define LOCATION_DIRECTIVE 8
#define AUTOINDEX_DIRECTIVE 9
#define LIMITEXCEPT_DIRECTIVE 10
#define CLIENTMAXBODYSIZE_DIRECTIVE 11
#define ALIAS_DIRECTIVE 12
#define REWRITE_DIRECTIVE 13
#define FASTCGIPASS_DIRECTIVE 14

class ADirective
{
	protected:
		// typedef	vector<ADirective*>::iterator	DirIt;

		std::string					_type;
		uint16_t					_context;
		std::vector<ADirective *>	_blocks;

		std::map<std::string, ADirective *>	_directives;

	public:
		ADirective();
		ADirective(const std::string &type, int context);
		ADirective(const ADirective &copy);
		virtual ~ADirective();
		ADirective &operator=(const ADirective &other);
		virtual ADirective *clone() const = 0;

		const std::string&						getType() const;
		std::vector<ADirective *>&				getBlocks();
		std::map<std::string, ADirective *>&	getDirectives();
		size_t									getBlocksSize() const;

		void	addBlock(ADirective *block);
		void	addDirective(ADirective *directive);
		void	addDirectiveRouter(const std::string &type);
};
