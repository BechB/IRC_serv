/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:27:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/23 18:00:35 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"
#include "headers/Client.hpp"
#include "headers/Channel.hpp"

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <limits>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>


Server::Server(int argc, char **argv)
{
    struct sockaddr_in addr;
//Pour l’utiliser dans une sockaddr_in, tu dois le convertir au format big-endian réseau

	if (argc != 3)
	{
		std::cout << "ERROR: need 2 arguments" << std::endl;
		exit(1);
	}
	for(int i = 0; argv[1][i]; i++)
	{
		if(!std::isdigit(argv[1][i]))
		{
			std::cout << "ERROR: check port format" << std::endl;
			exit(1);
		}

	}
	if(handle_port(argv[1]) == 1)
		exit(1);
	else
		_port = handle_port(argv[1]);
	if(handle_password(argv[2]) == 1)
		exit(1);
    _password = argv[2];
	addr.sin_port = htons(_port);
	addr.sin_family = AF_INET; //pour que mon socket soit defeini en tant que IPV4
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
	{
        std::cerr << "Error: socket creation failed" << std::endl;
		exit(1);
	}
	else
    std::cout << "OK" << std::endl;
    int value;
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)); //parametre le serveur pour que l ip soit utilisable sans delai
	fcntl(_sockfd, F_SETFL, O_NONBLOCK); //rend le socket non bloquant pour plusieurs client,
	perror("");
	bind(_sockfd, ((struct sockaddr *)&addr), sizeof(addr));
	perror("");
	listen(_sockfd, 15); //limiter le nombre de client qui peuvent se connecter (ici : 15)
						//permet d ecouter
    initSystemMsgs();
}

Server::~Server() 
{
	std::map<int, Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
		it->second.closeFd();
}

int    Server::handle_port(char *port)
{
	char *endptr;
	errno = 0;

	long val = strtol(port, &endptr, 10);
	if (errno == ERANGE || val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min()) {
		std::cout << "ERROR: check port format (int limits)" << std::endl;
		return 1;
	}
	if (*endptr != '\0')
	{
		std::cout << "ERROR: check port format" << std::endl;
		return 1;
	}
	
	if (val > 0)
	{
		if(val < 1024)
		{
			std::cout << "ERROR: reserved port" << std::endl;
			return 1;
		}
		if (val > 65535)
		{
			std::cout << "ERROR: port out of TCP range, need port between 1024 and 65535" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "ERROR: port can't be negatif" << std::endl;
		return 1;  
	}
	return val;
}

int Server::handle_password(char *password)
{
	int upper_count = 0;
	int lower_count = 0;
	int digit_count = 0;
	int special_count = 0;

	for(int i = 0; password[i]; i++)
	{
		if(isspace(password[i]))
		{
			std::cout << "ERROR: no space allowed in password" << std::endl;
			return 1;
		}
		if(isupper(password[i]))
			upper_count++;
		else if(islower(password[i]))
			lower_count++;
		else if(isdigit(password[i]))
			digit_count++;
		else
			special_count++;
	}
	if(password[0] == '\0')
	{
		std::cout << "ERROR: empty password, not secure, maybe secure, maybe not" << std::endl;
		return 1;
	}
	if(upper_count == 0)
	{
		std::cout << "ERROR: need at least one uppercase" << std::endl;
		return 1;
	}
	if(lower_count == 0)
	{
		std::cout << "ERROR: need at least one lowercase" << std::endl;
		return 1;
	}
	if(digit_count == 0)
	{
		std::cout << "ERROR: need at least one digit" << std::endl;
		return 1;
	}
	if(special_count != 0)
	{
		std::cout << "ERROR: no special caractere allowed" << std::endl;
		return 1;
	}
	return 0;
}

int Server::Routine()
{
    fd_set all_fds;
    FD_ZERO(&all_fds);        // Vide le set
    FD_SET(_sockfd, &all_fds); // Ajoute le socket serveur
	int max_fd = _sockfd;
	char buffer[4096];

	//fd_set read_fds = all_fds; // Copie de l'ensemble principal
	while(1)
	{
		fd_set read_fds = all_fds;
		int ready = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
		//apres ca, read_fds ne contient que les socket pret
		if(ready == -1)
		{
			std::cerr << "Error: request not accepted" << std::endl;
			return 1;
		}

		//vrifier si le socket serveur est prêt
		if (FD_ISSET(_sockfd, &read_fds))
		{
			//nouveau client en attente
			int client_fd = accept(_sockfd, NULL, NULL);
			std::cout << "client_fd: " << client_fd << std::endl;
			//ajouter le client au set et au container
			FD_SET(client_fd, &all_fds);
			if (client_fd > max_fd)
				max_fd = client_fd;

			Client newUser(client_fd);
			_clients.insert(std::make_pair(client_fd, newUser));

		}
		//vrifier les sockets des clients
		std::map<int, Client>::iterator it = _clients.begin();
		for (; it != _clients.end(); )
		{
			int client_fd = it->first;
    		Client& currentClient = it->second;

			/*
			fd_set est une structure fixe et limitée par FD_SETSIZE (souvent 1024).
			Quand un client se déconnecte, tu dois facilement retirer son descripteur de la liste.
			Avec un std::vector<int> ou std::set<int>, tu peux :
			Ajouter un client_fd après accept().
			Supprimer un client_fd quand recv() retourne 0 (client déconnecté).
			Boucler sur tous les clients actifs pour remplir ton fd_set avant l’appel à select().
			
			Si FD_ISSET(server_fd, &read_fds) → accept() → clients.push_back(new_client_fd)
			Pour chaque client_fd dans clients :
			Si FD_ISSET(client_fd, &read_fds) → recv()
			Si recv() == 0 (déconnexion) → close(client_fd) + retirer du clients.
			*/
			if (FD_ISSET(client_fd, &read_fds)) 
			{
				//le client a envoye des donnees
				
                memset(buffer, '\0', 4096);

                int signal = recv(client_fd, buffer, sizeof(buffer), 0);
                
				if (signal <= 0)
				{
					if (signal < 0)
						std::cout << "Signal error with "<< currentClient.getNickname() << std::endl;
					else
						std::cout << currentClient.getNickname() << " are disconnected" << std::endl;
					
					currentClient.closeFd();
					FD_CLR(client_fd, &all_fds);
					_clients.erase(it++);
					continue;
				}
				else
				{
					std::string message(buffer);

					send(client_fd, "debug: ", 7, 0);
					send(client_fd, message.c_str(), message.size(), 0);
					// check USER, NICK et PASS
					std::cout << "[" << message.find("\n") << "]" << std::endl;
					for (size_t i = 0; i < message.size(); ++i)
					{
        				if (message[i] == '\r')
							message[i] = '\n';
					}
					message = message.substr(0, message.find('\n'));
					extractCmd(message);
					
					if (!currentClient.getIsRegistred())
					{
						if (checkAuthenticate(currentClient))
							continue;
					}
							
					if (isCommandUsed(currentClient))
						continue;
					/*
					std::string message(buffer);
					message = message.substr(0, message.find("\r")); // coupe à \r si telnet

					
					//enter password
					//enter nickname
					//enter user
					//write(client_fd, "Enter nickname : ", 17);
					//substr la commande
					//attendre la bonne commande 
					// client[i]._nickname = "";
					//le client doit ecrire son nickname des qu il se connect sinon erreur
					//premiere commande = nickname
					std::cout << "user number " << client_fd << " sent " << buffer << std::endl; 
					//faire les commandes
					*/
				}
			}
			++it;
		}
	}
	perror("");
}

int	Server::check_password(int client_fd, std::string buffer)
{
	std::string try_pass = buffer.substr(0, strlen(buffer.c_str()) - 1);
	write(client_fd, "Enter password : ", 17);
	std::cout << "'" << try_pass << "'" << " : " << "'" << _password << "'" << std::endl;
	if(try_pass == _password)
		return 0;
	else
		return 1;
}

std::map<int, Client> Server::getClients() const {return _clients;}

std::map<std::string, Channel> Server::getChannels() const{return _channels;}

void Server::initSystemMsgs()
{
	_passGranted = "Password accepted. Please choose a nickname with NICK <name>\n";
	_passDenied = "Wrong password. Try again\n";
	_passCmdInfo = "Please enter password with PASS <password>\n";
	
	_nickGranted = "Nickname accepted. Please enter username with USER <name>\n";
	_nickCmdInfo = "Please choose a nickname with NICK <nickname>\n";
	
	_userGranted = "Welcome to the IRC server!\n";
	_userCmdInfo = "Please set your username with USER <username>\n";
}


bool Server::checkAuthenticate(Client& currentClient)
{
	int fd = currentClient.getFd();

	if (!currentClient.getHasPass())
	{
		if (_cmd.first == "PASS")
		{
			if (_cmd.second == _password)
			{
				currentClient.setHasPass();
				send(fd, _passGranted.c_str(), _passGranted.size(), 0);
			}
			else
				send(fd, _passDenied.c_str(), _passDenied.size(), 0);
		}
		else
			send(fd, _passCmdInfo.c_str(), _passCmdInfo.size(), 0);

		return false;
	}

	if (!currentClient.getHasNick())
	{
		if (_cmd.first == "NICK")
		{
			currentClient.setHasNick();
			currentClient.setNickname(_cmd.second);
			send(fd, _nickGranted.c_str(), _nickGranted.size(), 0);
		} 
		else
			send(fd, _nickCmdInfo.c_str(), _nickCmdInfo.size(), 0);

		return false;

	}

	if (!currentClient.getHasUser())
	{
		if (_cmd.first == "USER")
		{
			currentClient.setHasUser();
			currentClient.setUsername(_cmd.second);
			send(fd, _userGranted.c_str(), _userGranted.size(), 0);
		}
		else
			send(fd, _userCmdInfo.c_str(), _userCmdInfo.size(), 0);
	
		return false;
	}

	if (currentClient.getIsRegistred())
	{
		std::cout << "Client  " << fd - 3 << " are connected" <<std::endl;
		std::cout << "Nickname: " << currentClient.getNickname() << std::endl;
		std::cout << "Username: " << currentClient.getUsername() << std::endl;
	}
	else
		std::cout << "Can not registered. Try again." << std::endl;
	
	return true;
}

bool Server::isCommandUsed(Client& currentClient)
{
	if (_cmd.first == "JOIN")
	{
		std::cout << "JOIN found!" << std::endl;
		addClientToChannel(currentClient);

		return true;
	}

	return false;
}

void Server::addClientToChannel(Client& currentClient)
{
	std::cout << "le channel va être créé !" << std::endl;
	Channel newChannel(_cmd.second, currentClient.getFd());
	_channels.insert(std::make_pair(_cmd.second, newChannel));
	currentClient.joinChannel(_cmd.second);
	// todo
}

void Server::extractCmd(const std::string& message)
{
	size_t space = message.find(' ');
	
	if (space == std::string::npos)
	{
		_cmd.first = message;
		_cmd.second = "";
	}
	else
	{
		_cmd.first = message.substr(0, space);
		_cmd.second = message.substr(space + 1);
		const std::string lastt = _cmd.second;
	}
	std::cout << "Commande extraite: [" << _cmd.first << "] avec [" << _cmd.second << "]" << std::endl; 
}

