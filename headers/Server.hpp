/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/31 16:27:22 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <utility> // std::pair
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // pour close()
#include <fcntl.h>
#include <cerrno>
#include <limits>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <cctype>
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

// miscelaneous
#define ERR_NEEDMOREPARAMS " :Not enough parameters" 			// 461 <command> :
#define ERR_UNKNOWNCOMMAND " :Unknown command" 					// 421 <command> :

// profile / authenticate
#define ERR_NONICKNAMEGIVEN " :No nickname given" 				// 431
#define ERR_ERRONEUSNICKNAME " :Erroneus nickname" 				// 432
#define ERR_NICKNAMEINUSE " :Nickname is already in use" 		// 433
#define ERR_NOTREGISTERED " :You have not registered" 			// 451
#define ERR_ALREADYREGISTRED " :You may not reregister" 		// 462
#define ERR_PASSWDMISMATCH " :Password incorrect" 				// 464

// channel
#define ERR_NORECIPIENT " :No recipient given (PRIVMSG)"		// 411 :
#define ERR_NOTEXTTOSEND " :No text to send"					// 412 :
#define ERR_NOSUCHNICK " :No such nick/channel" 				// 401 <nick> :
#define ERR_NOSUCHCHANNEL " :No such channel" 					// 403 <canal> :
#define ERR_CANNOTSENDTOCHAN " :Cannot send to channel" 		// 404 <canal> :
#define ERR_USERNOTINCHANNEL " :They aren't on that channel" 	// 441 <nick> <canal> :
#define ERR_NOTONCHANNEL " :You're not on that channel" 		// 442 <canal> :
#define ERR_USERONCHANNEL " :is already on channel" 			// 443 <nick> <canal> :
#define ERR_CHANNELISFULL " :Cannot join channel (+l)" 			// 471 <canal> :
#define ERR_INVITEONLYCHAN " :Cannot join channel (+i)" 		// 473 <canal> :
#define ERR_BADCHANNELKEY " :Cannot join channel (+k)" 			// 475 <canal> :
#define ERR_CHANOPRIVSNEEDED " :You're not channel operator" 	// 482 <canal> : 
#define ERR_TOOMANYTARGETS " :Duplicate recipients. No message delivered" // 407 <target> :

// mode
#define ERR_UNKNOWNMODE " :is unknown mode char to me" 			// 472 <char> :


class Client;
class Channel;

class Server
{
	private:
		int 								_port;
		int 								_sockfd;

		std::pair<std::string, std::string> _cmd;
		std::map<int, Client> 				_clients;
		std::map<std::string, Channel>		_channels;
		std::string							_name;
		std::string 						_password;

	public:
		Server(int argc, char **argv);
		~Server();

		// getters
		int								getClientFd(const std::string& nickname) const;
		std::string 					getName() const;
		std::map<int, Client> 			getClients() const;
		std::map<std::string, Channel>	getChannels() const;
		
		int 	Routine();
		int    	handlePassword(char *);
		int    	handlePort(char *);
		int		checkPassword(int, std::string);
		bool	isNiERR_TOOMANYTARGETSckExist(const std::string&);

		// COMMANDS
		void 	checkCommand(Client&);
		void	hxSignIn(Client& client, const std::string&);
		void 	extractCmd(const std::string&);
		void	handlePASS(Client&, const std::string&);
		void	handleNICK(Client&, const std::string&);
		void	handleUSER(Client&, const std::string&);
		void	handleJOIN(Client&, const std::string&);
		void	handlePART(Client&, const std::string&);
		void	handleKICK(const Client&, const std::string&);
		void	handlePRIVMSG(Client&, const std::string&);
		void	handleTOPIC(const Client&, const std::string&);
		void	handleINVITE(const Client&, const std::string&);
		void	handleWHO(const Client&, const std::string&);
		void 	handleMODE(const Client&, const std::string&);
		void 	handleQUIT(Client&);
		void	kMode(const Client&, Channel&, const std::vector<std::string>&, size_t&);
		void	tMode(const Client&, Channel&, const std::vector<std::string>&);
		void	iMode(const Client&, Channel&, const std::vector<std::string>&);
		void	oMode(const Client&, Channel&, const std::vector<std::string>&, size_t&);
		void	lMode(const Client&, Channel&, const std::vector<std::string>&, size_t&);

		// client
		std::map<int, Client>::iterator findClientByNick(const std::string& nickname);

		// channels
		void	createChannel(const std::string& channelName, const Client& currentClient);
		bool	checkChannelPermissions(const Client& client, const Channel& channel, const std::vector<std::string>& params) const;
		std::map<std::string, Channel>::iterator	findChannel(const std::string& channelName);

		// system msg
		void	broadcastNickChange(const Client& client);
		void	sendSystemMsg(const Client& client, const std::string& code, const std::string& errmsg) const;
		void	RPL_TOPIC(const Client& cli, const Channel& channel) const;
		void	RPL_NOTOPIC(const Client& client, const Channel& channel) const;
		void	RPL_NAMREPLY(const Client& client, const Channel& channel) const;
		void	RPL_ENDOFNAMES(const Client& client, const Channel& channel) const;
		void	RPL_INVITING(const Client& client, const std::string& guestName, int guestFd, const std::string& channelName) const;
};
