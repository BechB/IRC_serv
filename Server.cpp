/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:27:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/25 19:15:13 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"

// in createChannel()
static bool parseJoinParams(const std::string& params)
{
	std::string channelName = params;
	std::istringstream iss(channelName);

	// iss will stop after reaching the first white space (or end of string), then grab the word before
	if (!(iss >> channelName) || channelName.empty())
		return false;
	
	// search if another word is found
	std::string searchExtraWord;

	if (iss >> searchExtraWord)
		return false;

	return true;
}

// used in the commands logic
static std::vector<std::string> divideParams(const std::string& param)
{
	std::vector<std::string> params;
	std::istringstream ss(param);
	std::string word;
	std::string	lastParam;
	
	while (ss >> word)
	{
		if (word[0] == ':' && word.size() > 1)
		{
			word.erase(0, 1);
			params.push_back(":");
			std::getline(ss, lastParam);
			word += lastParam;
			params.push_back(word);
			break;
		}
		
		params.push_back(word);
		
		if (word == ":")
		{
			std::getline(ss, lastParam);
			params.push_back(lastParam);	
			break;
		}
	}

	return params;
}

// in handlePRIVMSG()
static bool findColon(std::string& msg)
{
	size_t i = 0;
	
	for (; i < msg.size() && msg[i] == ' '; ++i);

	if (msg[i] != ':')
		return false;
	
	msg.erase(0, i + 1); // erase all start spaces + the : found
	return true;
}



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

	if(handlePort(argv[1]) == 1)
		exit(1);
	else
		_port = handlePort(argv[1]);
	if(handlePassword(argv[2]) == 1)
		exit(1);

	_name = "ircserv";
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
}

Server::~Server() 
{
	std::map<int, Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
		it->second.closeFd();
}


// getters 
std::string 					Server::getName() const {return _name;}
std::map<int, Client> 			Server::getClients() const {return _clients;}
std::map<std::string, Channel>	Server::getChannels() const{return _channels;}


// members
int    Server::handlePort(char *port)
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


int Server::handlePassword(char *password)
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
		for (; it != _clients.end();)
		{
			int client_fd = it->first;
    		Client& client = it->second;
			
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
                memset(buffer, '\0', 500);

                int signal = recv(client_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
				
				std::string clientName = client.getNickname();
				if (clientName.empty())
					clientName = "a client without nickname/username";
				
				// ctrl C
				if (signal == 0 || client.getIsQuiting())
				{
					close(client_fd);
					FD_CLR(client_fd, &all_fds);
					_clients.erase(it++);
					std::cout << clientName <<" is disconnected" << std::endl;
					continue;
				}
                std::cout << "BUFFER [" << buffer << "]" << std::endl;
                std::cout << "signal: " << signal << std::endl;
                std::string message(buffer);

				// ctrl D
				if (message.find("\n") == std::string::npos)
				{
					client.updateIncompMsg(message);
					break;
				}
				else
				{
					message = client.getIncompleteMsg() + message;
					client.clearIncompMsg();
				}
				
                if (signal < 0)
                {
                    // bien virer le client de tous les channels. Donc boucler sur tous les channels dans client::_channels
                    // et Channell::removeClient(le client) 
                    client.closeFd();
                    FD_CLR(client_fd, &all_fds);
                    _clients.erase(it++);
                    continue;
                }
				else
				{
					// handle hexchat sign in
					if (isHexchatSignIn(message))
						//todo


					for (size_t i = 0; i < message.size(); ++i)
					{
						if (message[i] == '\r')
						message[i] = '\n';
					}
					message = message.substr(0, message.find('\n'));

					extractCmd(message);
					if (_cmd.first == "QUIT")
					{
						client.setIsQuiting();
						continue;
					}
					
					const bool wasRegister = client.getIsRegistred();
					checkCommand(client);
					const bool isNowRegister = client.getHasPass() && client.getHasNick() && client.getHasUser();
					
					if (!wasRegister && isNowRegister)
					{
						client.setRegistred();
						std::string welcomeMsg = " :Welcome to IRC server, " + client.getNickname() + "!";
						sendSystemMsg(client, "001", welcomeMsg);
					}

					if (!wasRegister && !isNowRegister)
					{
						const std::string& command = _cmd.first;
						
						if (command != "PASS" && command != "USER" && command != "NICK" && message != "CAP LS 302")
						{
							const std::string reply = ":" + _name + " " + "451" + ERR_NOTREGISTERED + "\r\n";
							send(client.getFd(), reply.c_str(), reply.size(), 0);
						}
					}
					
					// std::string message(buffer);
					// message = message.substr(0, message.find("\r")); // coupe à \r si telnet
				}
			}
			++it;
		}
	}
}


int	Server::checkPassword(int client_fd, std::string buffer)
{
	std::string try_pass = buffer.substr(0, strlen(buffer.c_str()) - 1);
	write(client_fd, "Enter password : ", 17);
	std::cout << "'" << try_pass << "'" << " : " << "'" << _password << "'" << std::endl;
	if(try_pass == _password)
		return 0;
	else
		return 1;
}

void Server::handlePASS(Client& client, const std::string& pass)
{
	if (client.getIsRegistred())
	{
		sendSystemMsg(client, "462", ERR_ALREADYREGISTRED);
		return;
	}

	if (pass.empty())
	{
		sendSystemMsg(client, "461",  "* PASS " ERR_NEEDMOREPARAMS);
		return;
	}

	if (pass != _password)
	{
		sendSystemMsg(client, "464", ERR_PASSWDMISMATCH);
		return;
	}

	client.setHasPass();
}

// reach all the channels of the client, and will notify each client in these same channels the nick change 
void Server::broadcastNickChange(const Client& client)
{
	const std::set<std::string>& channels = client.getChannels();
	std::set<std::string>::iterator channel_it = channels.begin();
	
	std::string reply = ":" + client.getOldNickname() + " NICK :" + client.getNickname() + "\r\n";

	for (; channel_it != channels.end(); ++channel_it)
	{
		std::map<std::string, Channel>::iterator search_it = _channels.find(*channel_it);
		if (search_it == _channels.end())
			continue;

		const std::set<int>& members = search_it->second.getMembers();
		for (std::set<int>::iterator member_it = members.begin(); member_it != members.end(); ++member_it)
			send(*member_it, reply.c_str(), reply.size(), 0);
	}
}

void Server::handleNICK(Client& client, const std::string& name)
{
	std::cout << "handleNICK: NICK trouvé" << std::endl;
	if (name.empty())
	{
		sendSystemMsg(client, "431", ERR_NONICKNAMEGIVEN);
		return;
	}

	if (!isValidName(name))
	{
		sendSystemMsg(client, "432", ERR_ERRONEUSNICKNAME);
		return;
	}

	if (isNickExist(name))
	{
		sendSystemMsg(client, "433", ERR_NICKNAMEINUSE);
		return;
	}
	
	std::cout << "NICK setted" << std::endl;
	client.setHasNick();
	client.setNickname(name);

	if (client.getIsRegistred())
		broadcastNickChange(client);
}

void Server::handleUSER(Client& client, const std::string& name)
{
	if (name.empty())
	{
		sendSystemMsg(client, "461", "* USER " ERR_NEEDMOREPARAMS);
		return;
	}

	if (client.getIsRegistred())
	{
		sendSystemMsg(client, "462", ERR_ALREADYREGISTRED);
		return;
	}

	client.setHasUser();
	client.setUsername(name);
}

void Server::checkCommand(Client& client)
{
	std::string& command = _cmd.first;
	

	
	
	if (command == "CAP")
		return;

	std::string& parameter = _cmd.second;

	if (command == "PASS")
	{
		handlePASS(client, parameter);
		return;
	}
	
	if (command == "USER")
	{
		handleUSER(client, parameter);
		return;
	}
	
	if (command == "NICK")
	{
		handleNICK(client, parameter);
		return;
	}
	
	if (command == "QUIT")
	{
		handleQUIT(client);
		return;
	}

	if (client.getIsRegistred())
	{
		
		if (command == "JOIN")
		{
			handleJOIN(client, parameter);
			return;
		}

		if (command == "PART")
		{
			handlePART(client, parameter);
			return;
		}

		if (command == "TOPIC")
		{
			handleTOPIC(client, parameter);
			return;
		}

		if (command == "MODE")
		{
			handleMODE(client, parameter);
			return;
		}

		if (command == "WHO")
		{
			handleWHO(client, parameter);
			return;
		}

		if (command == "INVITE")
		{
			handleINVITE(client, parameter);
			return;
		}

		if (command == "KICK")
		{
			handleKICK(client, parameter);
			return;
		}

		if (command == "PRIVMSG")
		{
			handlePRIVMSG(client, parameter);
			return;
		}
		else
			sendSystemMsg(client, "421", " " + command + ERR_UNKNOWNCOMMAND); // if no command in this function is used
	}
}

void Server::handlePART(Client& client, const std::string& param)
{
	if (param.empty())
	{
		sendSystemMsg(client, "461", " PART" ERR_NEEDMOREPARAMS);
		return;
	}
	
	const std::vector<std::string>& params = divideParams(param);
	const std::string& channelName = params[0];
	
	
	std::map<std::string, Channel>::iterator itChannel = findChannel(channelName);

	if (channelName.empty() || channelName[0] != '#' || itChannel == _channels.end())
	{
		sendSystemMsg(client, "403", + " " + channelName + ERR_NOSUCHCHANNEL);
		return;
	}

	int fd = client.getFd();
	Channel& channel = itChannel->second;

	if (!channel.isMember(fd))
	{
		sendSystemMsg(client, "442", + " " + channelName + ERR_NOTONCHANNEL);
		return;
	}
	
	const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " PART " + channelName + "\r\n";
	channel.broadcast(reply);
	
	channel.removeOperator(fd);
	channel.removeMember(fd);
	channel.removeGuest(fd);
	client.leaveChannel(channelName);
	
}

void 	Server::handleQUIT(Client& client)
{
	int fd = client.getFd();
	std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " QUIT :" + client.getNickname()+ " is disconnected" + "\r\n";
	const std::set<std::string>& channels = client.getChannels();
	std::set<std::string>::iterator itClientChannels = channels.begin();
	
	if (itClientChannels != channels.end())
	{
		for (; itClientChannels != channels.end(); ++itClientChannels)
		{
			std::map<std::string, Channel>::iterator itChannel = findChannel(*itClientChannels);
			if (itChannel != _channels.end())
			{
				Channel& channel = itChannel->second;
	
				channel.broadcast(reply);
				
				if (channel.isOperator(fd))
					channel.removeOperator(fd);

				channel.removeMember(fd);
			}		
		}
		
		client.setIsQuiting();
	}
}

void Server::handleKICK(const Client& client, const std::string& param)
{
	const std::vector<std::string>& params = divideParams(param);
	const std::string& channelName = params[0];
	const std::string& targetName = params[1];
	// comment are all params after params[1], splited by space

	if (params.size() < 2 || channelName.empty() || targetName.empty())
	{
		sendSystemMsg(client, "461", "KICK" ERR_NEEDMOREPARAMS);
		return;
	}
	
	if (channelName[0] != '#')
	{
		sendSystemMsg(client, "403", channelName + ERR_NOSUCHCHANNEL);
        return;
	}

	std::map<std::string, Channel>::iterator itChan = findChannel(channelName);
	if (itChan != _channels.end())
	{
		std::map<int, Client>::iterator itClient = findClientByNick(targetName);
		if (itClient == _clients.end())
		{
			sendSystemMsg(client, "401", " " + targetName + ERR_NOSUCHNICK);
			return;
		}
		
		Channel& channel = itChan->second;
		Client& target = itClient ->second;
		int targetFd = target.getFd();
		int clientFd = client.getFd();
		std::string comment = "";

		if (!channel.isMember(clientFd))
		{
			sendSystemMsg(client, "442", channelName + ERR_NOTONCHANNEL);
			return;
		}
		
		if (!channel.isOperator(clientFd))
		{
			sendSystemMsg(client, "482", channelName + ERR_CHANOPRIVSNEEDED);
			return;
		}
		
		if (!channel.isMember(targetFd))
		{
			sendSystemMsg(client, "441", targetName + " " + channelName + ERR_USERNOTINCHANNEL);
			return;
		}
		
		if (params.size() > 2)
		{
			for (size_t i = 2; i < params.size() ;++i)
			{
				comment += params[i];
				if (i != params.size() - 1)
					comment += " ";			
			}
		}
		if (comment == "")
			comment = "Kicked";

		const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " KICK " + channelName + " " + targetName + " "+ comment + "\r\n";
		channel.broadcast(reply);
		if (channel.isOperator(targetFd))
			channel.removeOperator(targetFd);

		channel.removeMember(targetFd);
		target.leaveChannel(channelName);
	}
	else
		sendSystemMsg(client, "403", channelName + ERR_NOSUCHCHANNEL);
}

std::map<std::string, Channel>::iterator Server::findChannel(const std::string& channelName)
{
	std::map<std::string, Channel>::iterator it_chan = _channels.find(channelName);
	return it_chan;
}

int Server::getClientFd(const std::string& nickname) const
{
	for (std::map<int, Client>::const_iterator it_client = _clients.begin(); it_client != _clients.end(); ++it_client)
	{
		if (it_client->second.getNickname() == nickname)
			return it_client->first;
	}

	return -1;
}

// channel can not exist, invitation remains valid 
void Server::handleINVITE(const Client& client, const std::string& param)
{
	const std::vector<std::string>& params = divideParams(param);
	const std::string& targetChannel = params[1];
	const std::string& targetNickname = params[0];
	
	if (targetChannel.empty() || targetNickname.empty() || params.size() < 2)
	{
		sendSystemMsg(client, "461", "INVITE" ERR_NEEDMOREPARAMS);
		return;
	}

	if (targetChannel[0] != '#')
	{
		sendSystemMsg(client, "403", targetChannel + ERR_NOSUCHCHANNEL);
        return;
	}
	
	if (!isNickExist(targetNickname))
	{
		sendSystemMsg(client, "401", targetNickname + ERR_NOSUCHNICK);
		return;
	}
	
	int guestFd = getClientFd(targetNickname);
	std::map<std::string, Channel>::iterator itChan = findChannel(targetChannel);
	if (itChan != _channels.end())
	{
		Channel& channel = itChan->second;

		if (!channel.isMember(client.getFd()))
		{
			sendSystemMsg(client, "442", targetChannel + ERR_NOTONCHANNEL);
			return;
		}

		if (channel.getInvitOnly() && !channel.isOperator(client.getFd()))
		{
			sendSystemMsg(client, "482", targetChannel + ERR_CHANOPRIVSNEEDED);
			return;
		}
		
		if (channel.isMember(guestFd))
		{
			sendSystemMsg(client, "443", targetNickname + ERR_USERONCHANNEL);
			return;
		}

		// if -i, dont need to save the guest in the set<int> channel::_guests
		if (channel.getInvitOnly())
			channel.addGuest(guestFd);
	}
	
	RPL_INVITING(client, targetNickname, guestFd, targetChannel);
}

void Server::RPL_INVITING(const Client& client, const std::string& guestName, int guestFd, const std::string& channelName) const
{
	std::string reply = ":" + _name + " 341 " +  client.getNickname() + " " + guestName + " " + channelName + "\r\n";
	send(client.getFd(), reply.c_str(), reply.size(), 0);

	std::string notify = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " INVITE " + guestName + " :" + channelName + "\r\n";
	send(guestFd, notify.c_str(), notify.size(), 0);
}

void Server::handleWHO(const Client& client, const std::string& param)
{
    std::string channelName = param;
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (channelName.empty() || channelName[0] != '#' || it == _channels.end())
    {
        sendSystemMsg(client, "403", channelName + ERR_NOSUCHCHANNEL);
        return;
    }

    // if (it == _channels.end())
    // {
    //     sendSystemMsg(client, "403", channelName + ERR_NOSUCHCHANNEL);
    //     return;
    // }

    RPL_NAMREPLY(client, it->second);
    RPL_ENDOFNAMES(client, it->second);
}

void Server::handleMODE(const Client& client, const std::string& param)
{
	// params 0 = channel name
	// params 1 = +|- modes
	// params 2 = optionnal args
	const std::vector<std::string>& params = divideParams(param);
	for (size_t i = 0; i < params.size(); ++i)
		std::cout << "DEBUG params[" << i + 0 << "] = " << params[i] << std::endl;

	if (params.empty())
	{
		sendSystemMsg(client, "461", " MODE" ERR_NEEDMOREPARAMS);
		return;
	}

	const std::string& channelName = params[0];
	
	if (channelName.empty() || channelName[0] != '#')
	{
		sendSystemMsg(client, "403", channelName + ERR_NOSUCHCHANNEL);
		return;
	}

	std::map<std::string, Channel>::iterator it_channel = _channels.find(channelName);
	if (it_channel == _channels.end())
	{
		sendSystemMsg(client, "442", channelName + ERR_NOTONCHANNEL);
		return;
	}
	
	Channel& channel = it_channel->second;

	// if only #channel in params (/MODE #channel)
	if (params.size() < 2)
	{
		std::string reply = ":" + _name + " 324 " + client.getNickname() + " " + channel.getName() + " " + channel.getModes() + "\r\n";
		send(client.getFd(), reply.c_str(), reply.size(), 0);
		return;
	}

	const std::string& modes = params[1];

	if (modes[0] != '+' && modes[0] != '-')
	{
		sendSystemMsg(client, "461", "MODE" ERR_NEEDMOREPARAMS);
		return;
	}

	if (!channel.isOperator(client.getFd()))
	{
		sendSystemMsg(client, "482", channel.getName() + ERR_CHANOPRIVSNEEDED);
		return;
	}

	std::cout << "modes: " << modes << std::endl;
	size_t counter = 2; // if +okl <nick> <pass> <limit>, will associate o to nick (then counter++), k to pass (counter++) and l to limit
	// i = 1 -> skip the '+' / '-'
	for (size_t i = 1; i < modes.size(); ++i)
	{
		std::cout << "mode actuel: " << modes[i] << std::endl;
		if (modes[i] == 'k')
			kMode(client, channel, params, counter);
		else if (modes[i] == 't')
			tMode(client, channel, params);
		else if (modes[i] == 'i')
			iMode(client, channel, params);
		else if (modes[i] == 'o')
			oMode(client, channel, params, counter);
		else if (modes[i] == 'l')
			lMode(client, channel, params, counter);
		else
		{
			sendSystemMsg(client, "472", std::string(0, modes[i]) + ERR_UNKNOWNCOMMAND);
			return;
		}
	}
}

void Server::iMode(const Client& client, Channel& channel, const std::vector<std::string>& params)
{
	std::string operation;

	std::cout << "DEBUG i 1" << std::endl;
	if ( params[1][0] == '-')
		operation = "-i";
	else
		operation = "+i";
	std::cout << "DEBUG i 2" << std::endl;
	if (params.size() < 2)
	{
		sendSystemMsg(client, "461", " MODE " + operation + ERR_NEEDMOREPARAMS);
		return;
	}
	std::cout << "DEBUG i 3" << std::endl;
	if (params[1][0] == '-')
		channel.removeInvitOnly();
	else
		channel.enableInvitOnly();
	std::cout << "DEBUG i 4" << std::endl;
	bool irestrict = channel.getInvitOnly() ? true : false;
	const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " MODE " + channel.getName() + (irestrict ? " +i\r\n" : " -i\r\n");
	channel.broadcast(reply);
	std::cout << "DEBUG i 5" << std::endl;
}

std::map<int, Client>::iterator Server::findClientByNick(const std::string& nickname)
{
	std::map<int, Client>::iterator it_client = _clients.begin();

	for(; it_client != _clients.end(); ++it_client)
	{
		if (it_client->second.getNickname() == nickname)
			break;
	}
	return it_client;
}

void Server::oMode(const Client& client, Channel& channel, const std::vector<std::string>& params, size_t& counter)
{
	// params 0 = channel
	// params 1 = option
	// params 2 = target
	const std::string& target = params[counter];
	std::string operation;

	if ( params[1][0] == '+')
		++counter; // even if is an error, we must compare the next params[counter] to the right mode
		
	if ( params[1][0] == '-')
		operation = "-o";
	else
		operation = "+o";
	
	if (params[0][0] != '#' || target.empty())
	{
		sendSystemMsg(client, "461", " MODE " + operation + ERR_NEEDMOREPARAMS);
		return;
	}

	std::map<int, Client>::const_iterator it_client = findClientByNick(target);
	if (it_client == _clients.end())
	{
		sendSystemMsg(client, "401", " MODE " + operation + " " + target +  ERR_NOSUCHNICK);
		return;
	}
	
	int client_fd = it_client->first;
	
	if (!channel.isMember(client_fd))
	{
		sendSystemMsg(client, "441", " MODE " + operation + " " + target + " " + channel.getName() + ERR_USERNOTINCHANNEL);
		return;
	}
	
	if (params[1][0] == '-')
	{
		if (!channel.isOperator(client_fd))
			return;
		
		channel.removeOperator(client_fd);
		const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " MODE " + channel.getName() + " -o " + target + "\r\n";
		channel.broadcast(reply);
		return;
	}

	if (channel.isOperator(client_fd))
		return;

	channel.addOperator(client_fd);
	const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " MODE " + channel.getName() + " +o " + target + "\r\n";
	channel.broadcast(reply);
}

void Server::lMode(const Client& client, Channel& channel, const std::vector<std::string>& params, size_t& counter)
{
	std::string operation;
	std::cout << "COUNTER: " << counter << std::endl;
	
	std::cout << "DEBUG 1" << std::endl;

	if ( params[1][0] == '+')
		++counter; // even if is an error, we must compare the next params[counter] to the right mode

	if ( params[1][0] == '-')
		operation = "-l";
	else
		operation = "+l";

	if (params[counter - 1].empty() && params[1][0] == '+')
	{
		sendSystemMsg(client, "461", " MODE " + operation + ERR_NEEDMOREPARAMS);
		return;
	}

	const std::string& limit = params[counter - 1];

	if (params[1][0] == '-')
	{
		channel.removeLimit();
		const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " MODE " + channel.getName() + " -l\r\n";
		channel.broadcast(reply);
		return;
	}
	std::cout << "DEBUG 3" << std::endl;

	if (!isOnlyDigit(limit) || limit == "0" || limit[0] == '-' || limit.size() > 3)
	{
		sendSystemMsg(client, "461", " MODE " + operation + ERR_NEEDMOREPARAMS);
		return;
	}
	std::cout << "DEBUG 4" << std::endl;

	channel.setLimit(std::atoi(limit.c_str()));
	std::string reply = ":" + client.getNickname() + "!" + client.getUsername() +  "@" + _name + " MODE " + channel.getName() + " +l " + limit + "\r\n";
	channel.broadcast(reply);
}

void Server::tMode(const Client& client, Channel& channel, const std::vector<std::string>& params)
{
	std::string operation;

	if ( params[1][0] == '-')
		operation = "-t";
	else
		operation = "+t";

	if (params.size() < 2)
    {
        sendSystemMsg(client, "461", " MODE " + operation + ERR_NEEDMOREPARAMS);
        return;
    }

	// check if already +t in channel
	bool isRestricted = channel.getTopicRestriction() ? true : false;
	if (params[1][0] == '+')
	{
		if (isRestricted)
			return;
	}

	if (params[1][0] == '-')
		channel.removeTopicRestriction();
	else
		channel.enableTopicRestriction();

	isRestricted = channel.getTopicRestriction() ? true : false;
	const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " MODE " + channel.getName() + (isRestricted ? " +t\r\n" : " -t\r\n");
	channel.broadcast(reply);
}

void Server::kMode(const Client& client, Channel& channel, const std::vector<std::string>& params, size_t& counter)
{
	// params 0 = channel name
	// params 1 = +|- modes
	// params 2 = key 
	const std::string& password = params[counter];
	if (params[1][0] == '+')
		++counter;
	
	if (params[1][0] == '-')
	{
		channel.setKey("");
		const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " MODE " + channel.getName() + " -k\r\n";
		channel.broadcast(reply);
		return;
	}

	if (params.size() <= 2 || password.empty())
	{
		sendSystemMsg(client, "461", " MODE +k" ERR_NEEDMOREPARAMS);
		return;
	}

	if (params[1][0] == '-' && password == channel.getKey())
		return;

	channel.setKey(password);
	const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() +  "@" + _name + " MODE " + channel.getName() + " +k " + password + "\r\n";
	channel.broadcast(reply);

}


void Server::handleTOPIC(const Client& client, const std::string& param)
{
	const std::vector<std::string>& params = divideParams(param);
	const std::string& channelName = params[0];
	
	
	if (channelName.empty() || channelName[0] != '#')
	{
		sendSystemMsg(client, "403", channelName + ERR_NOSUCHCHANNEL);
		return;
	}
	
	std::map<std::string, Channel>::iterator itChannel = findChannel(channelName);
	
	if (itChannel == _channels.end())
	{
		sendSystemMsg(client, "442", channelName + ERR_NOTONCHANNEL);
		return;
	}
	
	Channel& channel = itChannel->second;
	
	if (params.size() < 2)
	{
		if (channel.getTopic().empty())
			RPL_NOTOPIC(client, channel);
		else
			RPL_TOPIC(client, channel);

		return;
	}

	
	if (channel.getTopicRestriction() && !channel.isOperator(client.getFd()))
	{
		sendSystemMsg(client, "482", channel.getName() + " " + ERR_CHANOPRIVSNEEDED);
		return;
	}
	
	std::string topic;
	for (size_t i = 1; i < params.size(); ++i)
		topic += params[i];
	
	if (params.size() < 3 || !findColon(topic))
	{
		sendSystemMsg(client, "461", " TOPIC" ERR_NEEDMOREPARAMS);
		return;
	}

	channel.setTopic(topic);

	const std::set<int>& members = channel.getMembers();
	std::set<int>::iterator it_member = members.begin();
	
	// envoyer RPL apres changement de topic a tous les clients du channel
	for (; it_member != members.end(); ++it_member)
	{		
		std::map<int, Client>::const_iterator it_map = _clients.find(*it_member);
		if (it_map != _clients.end())
		{
			RPL_TOPIC(it_map->second, channel);
		}
	}
}

bool Server::isNickExist(const std::string& nickname)
{
	std::map<int, Client>::iterator it = _clients.begin();

	for (; it != _clients.end(); ++it)
	{
		if (it->second.getNickname() == nickname)
			return true;
	}

	return false;
}


void Server::handlePRIVMSG(Client& client, const std::string& param)
{
	if (param.empty())
	{
		sendSystemMsg(client, "411", ERR_NORECIPIENT);
		return;
	}
	
	bool isClient = false;
	bool isChannel = false;

	std::map<int, Client>::iterator itClient;
	std::map<std::string, Channel>::iterator itChannel;
	const std::vector<std::string>& params = divideParams(param);
	const std::string& target = params[0];
	
	if (!target.empty() && target[0] == '#')
	{
		itChannel = findChannel(target);
		if (itChannel == _channels.end())
		{
			sendSystemMsg(client, "404", target + ERR_CANNOTSENDTOCHAN);
			return;
		}
	
		isChannel = true;
	}
	else
	{
		itClient = findClientByNick(target);
		if (itClient == _clients.end())
		{
			sendSystemMsg(client, "401", target + ERR_NOSUCHNICK);
			return;
		}
		
		isClient = true;
	}

	std::string msg;
	for (size_t i = 1; i < params.size(); ++i)
		msg += params[i];

	if (params.size() < 3 || !findColon(msg))
	{
		sendSystemMsg(client, "412", ERR_NOTEXTTOSEND);
		return;
	}
	
	const std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " PRIVMSG " + target + " :" + msg + "\r\n";
	if (isChannel)
		itChannel->second.broadcast(reply, client.getFd()); // broadcast(reply, exceptId) <- prevent to send to myself the reply

	else if (isClient)
		send(itClient->first, reply.c_str(), reply.size(), 0);
}


void Server::handleJOIN(Client& client, const std::string& param)
{	
	const std::vector<std::string>& params = divideParams(param);
	const std::string& channelName = params[0];
	
	if (!channelName.empty() && channelName[0] != '#')
	{
		sendSystemMsg(client, "403", + " " + channelName + ERR_NOSUCHCHANNEL);
		return;
	}

	if (channelName.empty() || channelName == "#")
	{
		sendSystemMsg(client, "461", " JOIN" ERR_NEEDMOREPARAMS);
		return;
	}

	std::map<std::string, Channel>::iterator itChannel = findChannel(channelName);
	
    if (itChannel == _channels.end())
		createChannel(channelName, client); // client will be added in the channel's ctor (in _members & _operators)
	else
	{
		if (itChannel->second.isMember(client.getFd()))
			return;

		if (!checkChannelPermissions(client, itChannel->second, params))
			return;

		itChannel->second.addMember(client.getFd());
	}
	client.joinChannel(channelName);

	itChannel = findChannel(channelName);
	
	// +i is checked in checkChannelPermissions() and when a guest join a +i server , we must remove it from channel::_guests
	if (itChannel->second.getInvitOnly())
		itChannel->second.removeGuest(client.getFd());
	
	const std::string reply = ":" + client.getNickname() + " JOIN " + channelName + "\r\n";
	itChannel->second.broadcast(reply);
	if (itChannel->second.getTopic().empty())
		RPL_NOTOPIC(client, itChannel->second);
	else
		RPL_TOPIC(client, itChannel->second);

	handleWHO(client, channelName);
}

bool Server::checkChannelPermissions(const Client& client, const Channel& channel, const std::vector<std::string>& params) const
{
	int limit = channel.getMemberLimit();
	const std::set<int>& members = channel.getMembers();
	
	if (limit > 0 && (((int)members.size() + 1) > limit))
	{
		sendSystemMsg(client, "471", + " " + channel.getName() + ERR_CHANNELISFULL);
		return false;
	}

	if (channel.getInvitOnly())
	{
		const std::set<int>& guests = channel.getGuest();
		std::set<int>::const_iterator client_fd = guests.find(client.getFd());
		if (client_fd == guests.end())
		{
			sendSystemMsg(client, "473", + " " + channel.getName() + ERR_INVITEONLYCHAN);
			return false;
		}
	}

	if (!channel.getKey().empty())
	{
		// params[0] = #chan, params[1] = key (optionnal)
		if (params.size() != 2 || !channel.checkKey(params[1]))
		{
			sendSystemMsg(client, "475", + " " + channel.getName() + ERR_BADCHANNELKEY);
			return false;
		}
	}

	return true;
}

void Server::createChannel(const std::string& channelName, const Client& currentClient)
{
	if (!parseJoinParams(_cmd.second))
	{
		sendSystemMsg(currentClient, "461", + " " + channelName + ERR_NEEDMOREPARAMS);
		return;
	}
	Channel newChannel(channelName, currentClient.getFd());
	_channels.insert(std::make_pair(channelName, newChannel));
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
	}
}

void Server::sendSystemMsg(const Client& client, const std::string& code, const std::string& errmsg) const
{
	std::string nick = client.getNickname().empty() ? "unknown user" : client.getNickname();

	std::string reply = ":" + _name + " " + code + " " + nick + errmsg + "\r\n";
	send(client.getFd(), reply.c_str(), reply.size(), 0);
}

void Server::RPL_TOPIC(const Client& client, const Channel& channel) const
{
	std::string reply = ":" + _name + " 332 " +  client.getNickname() + " " + channel.getName() + " :" + channel.getTopic() + "\r\n";
	
	send(client.getFd(), reply.c_str(), reply.size(), 0);
}

void Server::RPL_NOTOPIC(const Client& client, const Channel& channel) const
{
	std::string reply = ":" + _name + " 331 " +  client.getNickname() + " " + channel.getName() + " " + "No topic is set\r\n";
	send(client.getFd(), reply.c_str(), reply.size(), 0);
}

void Server::RPL_NAMREPLY(const Client& client, const Channel& channel) const
{
	// '=' is 'public channel'
	std::string reply = ":" + _name + " 353 " + client.getNickname() + " = " + channel.getName() + " :";

	const std::set<int>& clients = channel.getMembers();
	const std::set<int>& operators = channel.getOperators();
	std::set<int>::iterator it_client = clients.begin();

	for (; it_client != clients.end(); ++it_client)
	{
		std::string isOpe = "";
		std::map<int, Client>::const_iterator it_map = _clients.find(*it_client); // find client in _clients map of Server
	
		if (it_map != _clients.end())
		{	
			if (operators.find(it_map->first) != operators.end())
				isOpe = "@";

			reply += isOpe + it_map->second.getNickname() + " ";
		}
	}
	reply += "\r\n";
	send(client.getFd(), reply.c_str(), reply.size(), 0);
}

void Server::RPL_ENDOFNAMES(const Client& client, const Channel& channel) const
{
    std::string reply = ":" + _name + " 366 " + client.getNickname() + " " + channel.getName() + " :End of /NAMES list\r\n";
    send(client.getFd(), reply.c_str(), reply.size(), 0);
}
