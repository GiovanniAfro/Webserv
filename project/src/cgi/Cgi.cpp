/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 04:12:07 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/26 14:09:35 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

#include "Cgi.hpp"

Cgi::Cgi(Request &req, std::string path_info) {
    this->_params["SCRIPT_FILENAME"] = "";
    this->_params["QUERY_STRING"] = req.requestBody;
    this->_params["REQUEST_METHOD"] = req.request["method"];
    this->_params["CONTENT_TYPE"] = req.requestHeaders["Content-Type"];
    this->_params["CONTENT_LENGTH"] = req.requestHeaders["Content-Length"];
    this->_params["SCRIPT_NAME"] = "";
    this->_params["REQUEST_URI"] = "";
    this->_params["DOCUMENT_URI"] = "";
    this->_params["DOCUMENT_ROOT"] = path_info;
    this->_params["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_params["REQUEST_SCHEME"] = "http";
    this->_params["HTTPS"] = "off";
    this->_params["GATEWAY_INTERF"] = "CGI/1.1";
    this->_params["SERVER_SOFTWARE"] = "webserv/1.0";
    this->_params["REMOTE_ADDR"] = "";
    this->_params["REMOTE_PORT"] = "";
    this->_params["REMOTE_USER"] = "";
    this->_params["SERVER_ADDR"] = "";
    this->_params["SERVER_PORT"] = "";
    this->_params["SERVER_NAME"] = "";

    std::cout << "DEBUG CGI:" << this->_params["REQUEST_METHOD"] << std::endl;
}

Cgi::~Cgi(void) {}

std::string Cgi::exec(void) {
    char *const argv[] = {NULL};
    char *envp[this->_params.size() + 1];
    int i = 0;
    
    for (std::map<std::string, std::string>::iterator iter = _params.begin(); iter != _params.end(); ++iter) {
        std::string env_str = iter->first + "=" + iter->second;
        envp[i] = strdup(env_str.c_str());
        i++;
    }
    envp[i] = NULL;

    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        Log::error("CGI: Error creating pipe");
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        Log::error("CGI: Error forking");
        return "";
    }
    else if (!pid) {
        close(pipe_fds[1]);

        if (dup2(pipe_fds[0], STDIN_FILENO) == -1) {
            Log::error("CGI: Error duplicating pipe file descriptor");
            return "";
        }

        close(pipe_fds[0]);

        // if (execve(this->_params["SCRIPT_FILENAME"], argv, envp) == -1) {
        //     Log::error("CGI: execve failed");
        //     return "";
        // }
    }
    else {
        close(pipe_fds[0]);
        if (write(pipe_fds[1], this->_params["QUERY_STRING"].c_str(), 
                  this->_params["QUERY_STRING"].size()) == -1) {
            Log::error("CGI: Error writing to pipe");
            return "";
        }
        close(pipe_fds[1]);
        int status;
        waitpid(pid, &status, 0);
    }
}
