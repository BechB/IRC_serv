/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:59:23 by aldalmas          #+#    #+#             */
/*   Updated: 2025/09/23 18:48:23 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <set>


class Channel
{
    private:
        int             _memberLimit;
        bool            _invitOnly;
        std::string     _key;
        std::string     _topic;
        std::string     _name;
        std::set<int>   _operators;
        std::set<int>   _members;

    public:
        Channel(const std::string &name, int creator_fd);

        int             getMemberLimit() const;
        bool            getInvitOnly() const;
        std::string     getKey() const;
        std::string     getTopic() const;
        std::string     getName() const;
        std::set<int>   getMembers() const;
        std::set<int>   getOperators() const;

        void            addMember(int client_fd);
};
