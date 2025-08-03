/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbousaad <bbousaad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:54:50 by bbousaad          #+#    #+#             */
/*   Updated: 2025/08/03 10:58:27 by bbousaad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
// #include "Command.hpp"
// #include "Client.hpp"
// #include "Server.hpp"

/*

le programme est lance comme ceci : ./ircserv <port> <password>

INITIALISER LE SERVEUR
1. parser les arguments ( <port>, <password> )
2.creer le socket d ecoute
3.mettre le socket en mode non bloquant
4.binder le socket a l adresse IP(0.0.0.0) et au port donne
5.ecouter avec listen()
6.ajouter le socket a une structure de poll()


le port dans un int
le mot de passe dans une std::string
Si tu as une classe Server, c’est le bon endroit pour les stocker.

*/

int    handle_port(char *port)
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

int handle_password(char *password)
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

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
//Pour l’utiliser dans une sockaddr_in, tu dois le convertir au format big-endian réseau


	if (argc != 3)
	{
		std::cout << "ERROR: need 2 arguments" << std::endl;
		return 1;
	}
	for(int i = 0; argv[1][i]; i++)
	{
		if(!std::isdigit(argv[1][i]))
		{
			std::cout << "ERROR: check port format" << std::endl;
			return 1;
		}

	}
	long port;
	if(handle_port(argv[1]) == 1)
		return 1;
	else
		port = handle_port(argv[1]);
	if(handle_password(argv[2]) == 1)
		return 1;

	int sockfd;

	addr.sin_port = htons(port);
	addr.sin_family = AF_INET; //pour que mon socket soit defeini en tant que IPV4
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		return 1;
	}
	else
		std::cout << "OK" << std::endl;
	fcntl(sockfd, F_SETFL, O_NONBLOCK); //rend le socket non bloquant pour plusieurs client,
	perror("");
	bind(sockfd, ((struct sockaddr *)&addr), sizeof(addr));
	perror("");
	listen(sockfd, 15); //limiter le nombre de client qui peuvent se connecter (ici : 15)
						//permet d ecouter
	/*
	accept doit etre utiliser dans un for pour boucler sur les clients 
	et dans un if() pour verifier si le socket sonne
	il sonne si :   client se connect
					client se deconnecte
					commande taper
	*/
   
	/*
	select() surveille plusieurs descripteurs (sockets) et te dit lesquels sont prêts pour :
	Lire (readfds)
	Écrire (writefds)
	Ou s’ils ont une erreur (exceptfds)
	*/
   fd_set all_fds;
   FD_ZERO(&all_fds);        // Vide le set
   FD_SET(sockfd, &all_fds); // Ajoute le socket serveur
   int max_fd = sockfd;
   char buffer[42000];
   
	std::vector<int> clients;
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
			   	int signal_serveur = recv(sockfd, buffer, sizeof(buffer), 0);
			   	if (signal_serveur <= 0)
			   	{
				   	std::cout << "Main server close" << std::endl;
				   	for (size_t i = 0; i < clients.size(); ++i)
				   	{
						close(client_fd);
						FD_CLR(client_fd, &all_fds);
						std::cout << "Client number : " << i << " ejected" << std::endl;
					}
					close(sockfd);
					FD_CLR(sockfd, &all_fds);
				}
				//le client a envoye des donnees
				int signal = recv(client_fd, buffer, sizeof(buffer), 0);
				if (signal <= 0)
				{
					std::cout << "Someoene are disconnected" << std::endl;
					close(client_fd);
					FD_CLR(client_fd, &all_fds);
					clients.erase(clients.begin() + i);
					i--;
				} else {
					//faire les commande
				}
			}
		}
	}
	perror("");

	/*
	Le socket serveur est maintenant passif. Il ne communique pas directement avec les clients.
	C’est accept() qui crée un nouveau socket pour chaque client.
	Imagine que listen() ouvre la porte de ton bureau et que les clients font la queue dehors.
	accept() est l’action d’accueillir un client dans ton bureau et de lui donner un "canal privé" pour discuter.
	*/

	/*
	sockfd : le socket serveur (celui qui écoute).
	addr : pointeur vers une structure sockaddr où accept() va stocker l’adresse du client (IP et port).
	addrlen : taille de la structure addr.
	*/
	
	
	/*
	sockfd : le descripteur retourné par socket()
	addr : une structure sockaddr (souvent castée depuis sockaddr_in)
	addrlen : la taille de cette structure (sizeof(struct sockaddr_in))
	*/

	/*
	avec un socket bloquant, le programme se fige et attendra le resultat de la commande.
	Avec O_NONBLOCK, les appels comme recv(), send(), accept(), connect() ne bloquent plus.
	Si l’opération n’est pas prête (exemple : pas de données à lire),
	l’appel échoue immédiatement avec le code d’erreur EWOULDBLOCK ou EAGAIN.
	*/

	/*
	Dans les sockets, il existe plusieurs types, dont les deux principaux sont :
	SOCK_STREAM → Utilise TCP (protocole fiable, orienté connexion, qui garantit l'ordre et la livraison des données).
	SOCK_DGRAM → Utilise UDP (protocole non fiable, non orienté connexion, plus rapide mais sans garantie).
	Type : SOCK_STREAM → TCP  (Transmission Control Protocol)
	
	Domaine : AF_INET pour IPv4 ou AF_INET6 si tu veux gérer IPv6
	1. Le "domaine" (ou "family") d'une socket
	Quand on crée une socket en C, Python ou d'autres langages, on doit indiquer dans quel "domaine" de communication
	elle va fonctionner. Le domaine définit quel protocole réseau sera utilisé (IPv4, IPv6, etc.).

	2. AF_INET4
	AF_INET = Address Family Internet (pour IPv4).
	Cela signifie que la socket utilisera des adresses IPv4 (par exemple : 192.168.1.10).
	Ce domaine attend des adresses (IP + port) dans le format struct sockaddr_in.

	3. AF_INET6
	AF_INET6 = Address Family Internet version 6.
	Cela signifie que la socket utilisera des adresses IPv6 (par exemple : 2001:0db8:85a3::8a2e:0370:7334).
	Ce domaine attend des adresses dans le format struct sockaddr_in6.
	
	*/

	//etape 1
	//parser le port, ex de port valide (1024-65535) OK
	//stocker le mdp pour la connection
	// appels systeme classiques : socket(AF_INET, SOCK_STREAM, 0)
	// mettre le socket en mode non bloquant :fcntl(sockfd, F_SETFL, O_NONBLOCK);
	// car on ne dois jamais faire de blocage I/O;

	//etape 2
	/*
	prepare une struct sockaddr_in
	adresse : INADDR_ANY(0.0.0.0) pour ecouter sur toutes les interfaces, on fait pas ca.
	port: htons(port) (tu dois convertir le port au format reseau)
	appelle bind(sockfd, ...) pour lier le coket a l adresse
	*/
   
	//etape 3 
	/*
	demarrer l ecoute avec listen()
	listen(sockfd, SOMAXCONN);
	cela prepare le socket a recevoir des connexions entrantes.
	SOMAXCONN est la taille max de la file d’attente du système pour les connexions en attente
	*/
   
	//etape 4
	/*
	Avec select()
	on prepare un fd_set, FD_SET(sockfd, &readfds)
	garde le sockfd comme max fd a surveiller
	*/
   
	//etape 5
	/*
	attendre des evenement (connexion ou message)
	reagir sans blocage
	gerer pplusieurs clients via ta structure d'evenement
	*/
   

	
	
}