/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/08 16:40:34 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <utility> // std::pair
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
#include <sstream>
#include "Client.hpp"
#include "Channel.hpp"

#define CMD_INFO_PASS "Please enter password with PASS <password>"
#define CMD_INFO_NICK "Please choose a nickname with NICK <nickname>"
#define CMD_INFO_USER "Please set your username with USER <username>"
#define PASS_DENIED "Wrong password. Try again"
#define PASS_GRANTED "Password accepted. Please choose a nickname with NICK <name>"
#define NICK_GRANTED "Nickname accepted. Please enter username with USER <name>"

// miscelaneous
#define ERR_NEEDMOREPARAMS ":Not enough parameters" // 461
#define ERR_UNKNOWNCOMMAND " :Unknown command" // 421

// profile / authenticate
#define ERR_NONICKNAMEGIVEN ":No nickname given" // 431
#define ERR_ERRONEUSNICKNAME ":Erroneus nickname" // 432
#define ERR_NICKNAMEINUSE ":Nickname is already in use" //433
#define ERR_NOTREGISTERED ":You have not registered" // 451
#define ERR_ALREADYREGISTRED ":You may not reregister" // 462
#define ERR_PASSWDMISMATCH ":Password incorrect" // 464

// channel
#define ERR_NOSUCHCHANNEL "JOIN :No such channel" // 403
#define ERR_NOTONCHANNEL ":You're not on that channel" // 442
#define ERR_CHANNELISFULL "JOIN :Cannot join channel (+l)" // 471
#define ERR_BADCHANNELKEY "JOIN :Cannot join channel (+k)" // 475
#define ERR_CHANOPRIVSNEEDED " :You're not channel operator" // 482 

// mode
#define ERR_UNKNOWNMODE " :is unknown mode char to me" // 472



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
		
		std::string 						_passGranted;
		std::string 						_passDenied;
		std::string 						_passCmdInfo;
		
		std::string 						_nickGranted;
		std::string 						_nickCmdInfo;
		
		std::string 						_userGranted;
		std::string 						_userCmdInfo;

	public:
		Server(int argc, char **argv);
		~Server();

		// getters
		std::string 					getName() const;
		std::map<int, Client> 			getClients() const;
		std::map<std::string, Channel>	getChannels() const;
		
		int 	Routine();
		int    	handle_password(char *password);
		int    	handle_port(char *port);
		int		check_password(int client_fd, std::string buffer);
		void	initSystemMsgs();
		bool	isNickExist(const std::string& nickname);

		
		// COMMANDS
		void 	checkCommand(Client& client);
		void 	extractCmd(const std::string& message);
		void	handlePASS(Client& client, const std::string& pass);
		void	handleNICK(Client& client, const std::string& name);
		void	handleUSER(Client& client, const std::string& name);
		void	handleJOIN(Client& client, const std::string& param);
		void	handleTOPIC(const Client& client, const std::string& param);
		void 	handleMODE(const Client& client, const std::string& param);
		void	kMode(const Client& client, const std::vector<std::string>& params, Channel& channel);

		// client
		bool    isValidName(const std::string& name) const;
		void	broadcastNickChange(const Client& client);

		// channels
		void	createChannel(const std::string& channelName, const Client& currentClient);
		bool	checkChannelPermissions(const Client& client, const Channel& channel) const;
		void	memberEnterChannel(const Client& client, int otherMemberFd, const Channel& channel) const;
		void	RPL_CHANNELMODEIS(const Client& client, const std::string& channelName);


		// system msg
		void	sendSystemMsg(const Client& client, const std::string& code, const std::string& errmsg) const;
		void	RPL_TOPIC(const Client& cli, const Channel& channel) const;
		void	RPL_NOTOPIC(const Client& client, const Channel& channel) const;
		void	RPL_NAMREPLY(const Client& client, const Channel& channel) const;
		void	RPL_ENDOFNAMES(const Client& client, const Channel& channel) const;
};
