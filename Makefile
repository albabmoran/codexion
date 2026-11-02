# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/19 13:05:58 by albben-a          #+#    #+#              #
#    Updated: 2026/10/28 10:34:43 by albben-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME   = codexion
HEADER = codexion.h

GREEN := \033[1;32m
RESET := \033[0m

CC     = cc
FLAGS  = -Wall -Wextra -Werror -pthread

SRC    = 

OBJ    = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN) -> $(NAME) compiled and ready.$(RESET)"

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(OBJ): $(HEADER)

clean:
	@rm -f $(OBJ)
	@echo "$(GREEN) -> Object files deleted.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN) -> Executable deleted.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
