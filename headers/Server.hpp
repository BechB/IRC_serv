/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/03 17:54:25 by aldalmas         ###   ########.fr       */
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

#define ERR_NONICKNAMEGIVEN " :No nickname given" // 431
#define ERR_CHANNELISFULL "JOIN :Cannot join channel (+l)" // 471
#define ERR_BADCHANNELKEY "JOIN :Cannot join channel (+k)" // 475
#define ERR_NOSUCHCHANNEL "JOIN :No such channel" // 403
#define ERR_NEEDMOREPARAMS_PASS "PASS :Not enough parameters" // 461
#define ERR_NEEDMOREPARAMS_USER "USER :Not enough parameters" // 461
#define ERR_NEEDMOREPARAMS_NICK "NICK :Not enough parameters" // 461
#define ERR_NEEDMOREPARAMS_CHAN "NICK :Not enough parameters" // 461
#define ERR_ALREADYREGISTRED " :You may not reregister" // 462
#define ERR_NICKCOLLISION " :Nickname collision KILL" //436



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
		bool 	isUserExist(const std::string& username);
		bool	isNickExist(const std::string& nickname);

		
		// COMMANDS
		void 	checkAuthenticate(Client& currentClient);
		bool 	isCommandUsed(Client& currentClient);
		void 	extractCmd(const std::string& message);
		void	addClientToChannel(Client& currentClient);

		// channels
		void	createChannel(const std::string& channelName, const Client& currentClient);
		bool	checkChannelPermissions(const Client& client, const Channel& channel) const;
		void	JOINmessage(const Client& client, const Channel& channel) const;
		void	memberEnterChannel(const Client& client, int otherMemberFd, const Channel& channel) const;

		// system msg
		void	sendSystemMsg(const Client& client, const std::string& code, const std::string& errmsg) const;
		void	RPL_TOPIC(const Client& cli, const Channel& channel) const;
		void	RPL_NOTOPIC(const Client& client, const Channel& channel) const;
		void	RPL_NAMREPLY();
};


