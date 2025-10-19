/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:19:35 by aldalmas          #+#    #+#             */
/*   Updated: 2025/10/19 17:33:43 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

bool isOnlyDigit(const std::string& str)
{
    if (str.empty())
        return false;

    std::cout << "DEBUG str: " << str << std::endl;

    for (size_t i = 0; i < str.size(); ++i)
    {
        std::cout << "DEBUG 3-a" << std::endl;
        if (!std::isdigit(str[i]) && str[i] != '-' && str[i] != '+')
            return false;
        std::cout << "DEBUG 3-b" << std::endl;
    }
    std::cout << "DEBUG 3-c" << std::endl;
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
