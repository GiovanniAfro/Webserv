/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 04:12:07 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/01 18:44:30 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(Request &req, string path_info) {
    split_path(path_info, this->_params["DOCUMENT_ROOT"],
               this->_params["SCRIPT_NAME"]);
    this->_params["SCRIPT_FILENAME"] = path_info;
    this->_params["REQUEST_METHOD"] = req.request["method"];
    this->_params["CONTENT_TYPE"] = req.requestHeaders["Content-Type"];
    this->_params["CONTENT_LENGTH"] = req.requestHeaders["Content-Length"];
    this->_params["REQUEST_URI"] = req.request["uri"];
    this->_params["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_params["REQUEST_SCHEME"] = "http";
    this->_params["HTTPS"] = "off";
    this->_params["GATEWAY_INTERF"] = "CGI/1.1";
    this->_params["SERVER_SOFTWARE"] = "webserv/1.0";
    if (this->_params["REQUEST_METHOD"] == "POST")
        this->_request_body = req.requestBody;
    else
        this->_request_body = "";
}

Cgi::~Cgi(void) {}

char **Cgi::_get_envp() {
    char **envp = new char *[this->_params.size() + 1];
    string env_str;
    int i = 0;

    for (map<string, string>::iterator it = this->_params.begin();
        it != this->_params.end(); ++it) {
        env_str = it->first + "=" + it->second;
        envp[i] = new char[env_str.length() + 1];
        strcpy(envp[i], env_str.c_str());
        i++;
    }
    envp[i] = NULL;
    return envp;
}

void Cgi::_free_envp(char **envp) {
    for (int i = 0; envp[i] != NULL; ++i)
        delete[] envp[i];
    delete[] envp;
}

void Cgi::_processing_child(char **envp, int *pipe_in, int *pipe_out) {
    char *const argv[] = {NULL};

    if (close(pipe_in[1]) == -1 || close(pipe_out[0]) == -1) {
        Log::error("CGI: Error close()");
        exit(EXIT_FAILURE);
    }
    if (dup2(pipe_in[0], STDIN_FILENO) == -1 ||
        dup2(pipe_out[1], STDOUT_FILENO) == -1 || 
        dup2(pipe_out[1], STDERR_FILENO) == -1) {
        Log::error("CGI: Error dup2()");
        exit(EXIT_FAILURE);
    }
    if (close(pipe_in[0]) == -1 || close(pipe_out[1]) == -1) {
        Log::error("CGI: Error close()");
        exit(EXIT_FAILURE);
    }
    if (chdir(this->_params["DOCUMENT_ROOT"].c_str()) == -1) {
            Log::error("CGI: Error chdir()");
            exit(EXIT_FAILURE);
        }
    if (execve(this->_params["SCRIPT_FILENAME"].c_str(), argv, envp) == -1)
        exit(EXIT_FAILURE);
}

pair<HTTP_STATUS, string> Cgi::_processing_father(char **envp, pid_t pid, int *pipe_in, int *pipe_out) {
    int status;
    ssize_t bytes_read;
    char buffer[4096];
    pair<HTTP_STATUS, string> out;

    this->_free_envp(envp);
    if (close(pipe_in[0]) == -1 || close(pipe_out[1]) == -1) {
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error close()";
        Log::error(out.second);
        return out;
    }
    if (write(pipe_in[1], this->_request_body.c_str(), 
              this->_request_body.length()) == -1) {
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error write()";
        Log::error(out.second);
        return out;
    }
    if (close(pipe_in[1]) == -1) {
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error close()";
        Log::error(out.second);
        return out;
    }
    while ((bytes_read = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
        out.second.append(buffer, bytes_read);
    if (bytes_read == -1) {
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error reading from pipe";
        Log::error(out.second);
        return out;
    }
    if (close(pipe_out[0]) == -1) {
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error close()";
        Log::error(out.second);
        return out;
    }
    if (waitpid(pid, &status, 0) == -1) {
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error waitpid()";
        Log::error(out.second);
        return out;
    }
    if (WIFEXITED(status))
        out.first = OK;
    else
        out.first = BAD_GATEWAY;
    return out;
}

pair<HTTP_STATUS, string> Cgi::exec(void) {
    int pipe_in[2], pipe_out[2];
    pid_t pid;
    char **envp = this->_get_envp();
    pair<HTTP_STATUS, string> out;

    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        Log::error("CGI: Error creating pipe");
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error creating pipe";
        return out;
    }
    pid = fork();
    if (pid == -1) {
        Log::error("CGI: Error forking");
        out.first = INTERNAL_SERVER_ERROR;
        out.second = "CGI: Error forking";
        return out;
    }
    else if (!pid)
        this->_processing_child(envp, pipe_in, pipe_out);
    else
        out = this->_processing_father(envp, pid, pipe_in, pipe_out);
    // cout << "STATUS_CODE: " << out.first << endl << "OUTPUT: " << out.second << endl; 
    return out;
}
