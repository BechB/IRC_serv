# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bech <bech@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/15 10:05:32 by bbousaad          #+#    #+#              #
#    Updated: 2025/08/03 15:27:11 by bech             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRC = main.cpp \
		Server.cpp \
		# Clien.cpp \
		# Command.cpp \

OBJ = $(SRC:.cpp=.o)

CC = c++
FLAGS =  -Wall -Wextra -Werror -std=c++98 -g

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@${CC} $(FLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	@rm -f $(OBJ)
	@echo "Suppression des .o" 

fclean: clean
	@rm -f $(OBJ) $(NAME)
	@echo "Suppression du .a effectuée"

re: fclean all