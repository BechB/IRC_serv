/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbousaad <bbousaad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/08/10 15:01:03 by bbousaad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>
#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>
#include <limits>
#include <cerrno>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>   // pour close()
#include <stdio.h>
#include <vector>
#include <cstring>
#include "Client.hpp"

class Client;

class Server
{
	private:
	
		Client  client[50];
		std::vector<int> clients;
		std::string _password;
		int _port;
		int sockfd;
		int fd;
		bool has_pass;
		bool has_nick;
		bool has_user;
		std::string nickname;
		std::string username;

	public:
		Server(int argc, char **argv);
		~Server();
		int 	Routine();
		int    	handle_password(char *password);
		int    	handle_port(char *port);
		int		check_password(int client_fd, std::string buffer);
	
};