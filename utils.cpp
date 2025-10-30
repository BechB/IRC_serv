/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:19:35 by aldalmas          #+#    #+#             */
/*   Updated: 2025/10/30 15:02:15 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

bool isHexchatSignIn(const std::string& s)
{
    if (s.find("PASS") != std::string::npos
        && s.find("USER") != std::string::npos 
        && s.find("NICK") != std::string::npos)
        return true;
    
    if (s.find("CAP LS 302") != std::string::npos
        && s.find("PASS") != std::string::npos
        && s.find("USER") != std::string::npos 
        && s.find("NICK") != std::string::npos)
        return true;

    return false;
}

bool isOnlyDigit(const std::string& str)
{
    if (str.empty())
        return false;

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]) && str[i] != '-' && str[i] != '+')
            return false;
    }

    return true;
}

bool isValidName(const std::string& name)
{
    const int maxSize = 10;
    const std::string authorizedChars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    if (name.empty() || name.size() > maxSize)
        return false;

    for (size_t i = 0; i < name.size(); ++i)
    {
        if (authorizedChars.find(name[i]) == std::string::npos)
            return false;
    }
    return true;
}
