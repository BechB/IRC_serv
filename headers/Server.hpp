/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/18 15:14:57 by aldalmas         ###   ########.fr       */
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


class Server
{
	private:
	
		// Client  client[50];
		// std::vector<int> clients; // à supprimer
		
		std::vector<Channel> _channels; // alex ajout
		std::vector<Client> _clients; // alex ajout
		
		std::string username;
		std::string nickname;
		std::string _password;
		int _port;
		int sockfd;
		int fd;

	public:
		Server(int argc, char **argv);
		~Server();

		std::vector<Client> getClients() const; // alex ajout
		std::vector<Channel> getChannels() const; // alex ajout
		
		int 	Routine();
		int    	handle_password(char *password);
		int    	handle_port(char *port);
		int		check_password(int client_fd, std::string buffer);
};