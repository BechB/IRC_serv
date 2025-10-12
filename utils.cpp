#include <iostream>

bool isOnlyDigit(const std::string& str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]) && str[i] != '-' && str[i] != '+')
            return false;
    }

    return true;
}