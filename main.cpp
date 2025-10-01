/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:54:50 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/29 17:07:33 by aldalmas         ###   ########.fr       */
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
#include "headers/Server.hpp"


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

int main(int argc, char **argv)
{
	Server serv(argc, argv);

	if(serv.Routine() == 1)
		return 1;
	return 0;
}
	
	
	
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
	/*
	Le socket serveur est maintenant passif. Il ne communique pas directement avec les clients.
	C’est accept() qui crée un nouveau socket pour chaque client.
	Imagine que listen() ouvre la porte de ton bureau et que les clients font la queue dehors.
	accept() est l’action d’accueillir un client dans ton bureau et de lui donner un "canal privé" pour discuter.
	*/

	/*
	_sockfd : le socket serveur (celui qui écoute).
	addr : pointeur vers une structure sockaddr où accept() va stocker l’adresse du client (IP et port).
	addrlen : taille de la structure addr.
	*/
	
	
	/*
	_sockfd : le descripteur retourné par socket()
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
	// mettre le socket en mode non bloquant :fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	// car on ne dois jamais faire de blocage I/O;

	//etape 2
	/*
	prepare une struct sockaddr_in
	adresse : INADDR_ANY(0.0.0.0) pour ecouter sur toutes les interfaces, on fait pas ca.
	port: htons(port) (tu dois convertir le port au format reseau)
	appelle bind(_sockfd, ...) pour lier le coket a l adresse
	*/
   
	//etape 3 
	/*
	demarrer l ecoute avec listen()
	listen(_sockfd, SOMAXCONN);
	cela prepare le socket a recevoir des connexions entrantes.
	SOMAXCONN est la taille max de la file d’attente du système pour les connexions en attente
	*/
   
	//etape 4
	/*
	Avec select()
	on prepare un fd_set, FD_SET(_sockfd, &readfds)
	garde le _sockfd comme max fd a surveiller
	*/
   
	//etape 5
	/*
	attendre des evenement (connexion ou message)
	reagir sans blocage
	gerer pplusieurs clients via ta structure d'evenement
	*/
   