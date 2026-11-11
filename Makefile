# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/19 13:05:58 by albben-a          #+#    #+#              #
#    Updated: 2026/11/11 21:13:17 by albben-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME   = codexion
HEADER = coders/codexion.h

GREEN := \033[1;32m
RESET := \033[0m

CC     = cc
FLAGS  = -Wall -Wextra -Werror -pthread
INCLUDES = -Icoders

SRC    = 	coders/main.c\
			coders/parsing.c\
			coders/initializer/initializer.c\
			coders/initializer/init_structs.c\
			coders/initializer/init_threads.c\
			coders/time.c\
			coders/routines/coder.c\
			coders/routines/coder_tasks.c\
			coders/routines/printer.c\
			coders/routines/monitor.c\
			coders/routines/req_manager.c\
			coders/routines/req_manager_tasks.c\
			coders/dongles.c\
			coders/heap.c\
			coders/cleanup.c\
			coders/stop_simulation.c\
			

OBJ    = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN) -> $(NAME) compiled and ready.$(RESET)"

%.o: %.c
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(OBJ): $(HEADER)

clean:
	@rm -f $(OBJ)
	@echo "$(GREEN) -> Object files deleted.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN) -> Executable deleted.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
