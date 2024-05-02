/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:25:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/01 18:42:46 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

class Cgi {
    private:
        std::map<std::string,std::string> _params;
        std::string _request_body;

        char **_get_envp(void);
        void _free_envp(char **envp);
        void _processing_child(char **envp, int *pipe_in, int *pipe_out);
        std::string _processing_father(char **envp, pid_t pid, int *pipe_in, int *pipe_out);

    public:
        Cgi(Request &req, std::string path_info);
		Cgi(const Cgi &copy);
		~Cgi();
		Cgi &operator=(const Cgi &other);

        std::string exec(void);
};
