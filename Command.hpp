/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbousaad <bbousaad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:07 by bbousaad          #+#    #+#             */
/*   Updated: 2025/07/15 16:04:41 by bbousaad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>
#include <limits>
#include <cerrno>

int    handle_port(char *port);