/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:59:23 by aldalmas          #+#    #+#             */
/*   Updated: 2025/09/17 16:02:58 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "Client.hpp"


class Client;

class Channel
{
    private:
        std::string         _name;
        std::vector<Client> _clients;

    public:
        std::string         getName() const;
        std::vector<Client> getClients() const;

};