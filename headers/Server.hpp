/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/23 17:48:03 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <utility> // std::pair


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

		
		std::map<int, Client> 			getClients() const;
		std::map<std::string, Channel>	getChannels() const;
		
		int 	Routine();
		int    	handle_password(char *password);
		int    	handle_port(char *port);
		int		check_password(int client_fd, std::string buffer);
		void	initSystemMsgs();
		
		// COMMANDS
		bool 	checkAuthenticate(Client& currentClient);
		bool 	isCommandUsed(Client& currentClient);
		void 	extractCmd(const std::string& message);
		void	addClientToChannel(Client& currentClient);
};