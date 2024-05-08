/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:25:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/07 21:03:37 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

using std::string;
using std::map;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;

// Class ---------------------------------------------------------------------->

class Cgi {
    private:
        map<string, string> _params;
        string              _request_body;
        // unsigned int        _timeout;

        char **_get_envp(void);
        void _free_envp(char **envp);
        void _processing_child(char **envp, int *pipe_in, int *pipe_out);
        pair<HTTP_STATUS, string> _processing_father(char **envp, pid_t pid, int *pipe_in, int *pipe_out);

    public:
        Cgi(Request &req, string path_info);
		Cgi(const Cgi &copy);
		~Cgi();
		Cgi &operator=(const Cgi &other);

        pair<HTTP_STATUS, string> exec(void);
};
