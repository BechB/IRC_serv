/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbousaad <bbousaad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:27:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/08/10 16:22:39 by bbousaad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
        std::cerr << "Error: socket creation failed" << std::endl;
		exit(1);
	}
	else
    std::cout << "OK" << std::endl;
    int value;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)); //parametre le serveur pour que l ip soit utilisable sans delai
	fcntl(sockfd, F_SETFL, O_NONBLOCK); //rend le socket non bloquant pour plusieurs client,
	perror("");
	bind(sockfd, ((struct sockaddr *)&addr), sizeof(addr));
	perror("");
	listen(sockfd, 15); //limiter le nombre de client qui peuvent se connecter (ici : 15)
						//permet d ecouter
    
}

Server::~Server() 
{
    for(int i = 0; i < (int)clients.size(); i++)
    {
        close(clients[i]);
    }
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
	if (*endptr != '\0') {
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
    FD_SET(sockfd, &all_fds); // Ajoute le socket serveur
	int max_fd = sockfd;
	char buffer[4096];

	//fd_set read_fds = all_fds; // Copie de l'ensemble principal
	while(5)
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
		if (FD_ISSET(sockfd, &read_fds))
		{
			//nouveau client en attente
			int client_fd = accept(sockfd, NULL, NULL);
			
			//ajouter le client au set et au container
			FD_SET(client_fd, &all_fds);
			if (client_fd > max_fd)
				max_fd = client_fd;  //new max_fd
			clients.push_back(client_fd);
		}
		//vrifier les sockets des clients
		for (size_t i = 0; i < clients.size(); ++i)
		{
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
			int client_fd = clients[i];
			if (FD_ISSET(client_fd, &read_fds)) 
			{
				//le client a envoye des donnees
				
                memset(buffer, '\0', 4096);

                int signal = recv(client_fd, buffer, sizeof(buffer), 0);
                
				if (signal <= 0)
				{
					std::cout << "Someoene are disconnected" << std::endl;
					close(client_fd);
					FD_CLR(client_fd, &all_fds);
					clients.erase(clients.begin() + i);
					i--;
				}
				else
				{
					std::string message(buffer);
					message = message.substr(0, message.find("\r"));
					std::cout << "user number " << client_fd << " sent " << message << std::endl;

					if (!has_pass) {
						if (message.rfind("PASS ", 0) == 0)
						{ // commence par "PASS "
							std::string pass = message.substr(5, _password.size());
							// std::cout << "PASSWORD CLIENT : " << pass << std::endl;
							// std::cout << "PASSWORD SERVER : " << _password << std::endl;
							if (pass == _password) { // mot de passe attendu
								has_pass = true;
								send(client_fd, "Password accepted. Please choose a nickname with NICK <name>\n", 61, 0);
							} else {
								send(client_fd, "Wrong password. Try again\n", 24, 0);
							}
						} else {
							send(client_fd, "Please enter password with PASS <password>\n", 44, 0);
						}
						continue; // on ne passe pas à la suite
					}

					if (!has_nick) {
						if (message.rfind("NICK ", 0) == 0) {
							nickname = message.substr(5);
							has_nick = true;
							send(client_fd, "Nickname accepted. Please enter username with USER <name>\n", 59, 0);
						} else {
							send(client_fd, "Please choose a nickname with NICK <nickname>\n", 46, 0);
						}
						continue;
					}
					client[i].client_nickname = nickname;

					if (!has_user) {
						if (message.rfind("USER ", 0) == 0) {
							username = message.substr(5);
							has_user = true;
							send(client_fd, "Welcome to the IRC server!\n", 28, 0);
						} else {
							send(client_fd, "Please set your username with USER <username>\n", 46, 0);
						}
						continue;
					}
					client[i].client_username = username;

					std::cout << "Client number " << i + 1 << " are connected" <<std::endl;
					std::cout << "Nickname: " << client[i].client_nickname << std::endl;
					std::cout << "Username: " << client[i].client_username << std::endl;
					
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