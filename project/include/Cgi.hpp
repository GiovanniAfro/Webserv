/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:25:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/01 12:14:06 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

class Cgi {
    private:
        std::map<std::string,std::string> _params;
        std::string _request_body;

    public:
        Cgi(std::map<std::string, std::string> &req, std::map<std::string, 
            std::string> req_head, std::string path_info);
		Cgi(const Cgi &copy);
		~Cgi();
		Cgi &operator=(const Cgi &other);

        std::string exec(void);
};
