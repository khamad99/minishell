# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/09 11:29:51 by ooutabac          #+#    #+#              #
#    Updated: 2023/06/06 23:07:33 by kalshaer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFT_DIR	=	libft
LIBFT		=	libft.a
RL_INC		=	-I /usr/local/opt/readline/include #-I /goinfre/include
RL_LINK		=	-L /usr/local/opt/readline/lib -l readline #-L /goinfre/lib -l readline

INC_DIR		=	include
SRC_DIR		=	src
OBJ_DIR		=	objects

SRCS		=	parsing/parse.c				\
				parsing/utils1.c			\
				parsing/utils2.c			\
				parsing/utils3.c			\
				parsing/utils4.c			\
				parsing/utils5.c			\
				parsing/utils6.c			\
				parsing/utils7.c			\
				parsing/free.c				\
				src/main.c					\
				builtin/echo.c				\
				builtin/env.c				\
				builtin/pwd.c				\
				builtin/unset.c				\
				builtin/unset_utils.c		\
				builtin/exit.c				\
				builtin/export.c			\
				builtin/export_utils1.c		\
				builtin/export_utils2.c		\
				builtin/export_utils3.c		\
				builtin/cd.c				\
				builtin/cd_utils.c			\
				builtin/cd_utils2.c			\
				builtin/builtin_utils.c		\
				src/excution.c				\
				src/error_handling.c		\
				src/exec_child.c			\
				src/exec_child_utils.c		\
				src/redir.c					\
				src/exec_heredoc.c			\
				src/exec_infile.c			\
				src/exec_outfile.c			\
				src/pipes.c					\
				src/signal.c				\
				parsing/skip.c				\
				parsing/count_redirections.c\
				#parsing/dollar_sign1.c		\#

OBJS		=	$(SRCS:%c=%o)

CC			=	gcc
CFLAGS		=	-g -Wall -Wextra -Werror #-fsanitize=address
RM			=	rm -rf

all:		$(NAME)
	@echo "           ████"
	@echo "         ▒▒▓▓▓▓▒▒"
	@echo "           ▓▓▓▓░░"
	@echo "           ▓▓▓▓                    ░░"
	@echo "     ▓▓████████████                ░░"
	@echo "     ░░██████████▓▓            ░░░░░░░░░░"
	@echo "       ▓▓████████▓▓            ░░░░░░░░░░            ▒▒"
	@echo "       ▒▒████████              ░░░░░░░░              ▓▓░░"
	@echo "         ██▓▓████                ░░░░░░              ▓▓  ▒▒            ░░░░"
	@echo "     ░░▓▓▓▓▓▓▓▓▓▓▒▒              ░░░░░░            ██▓▓▒▒▓▓          ░░░░░░░░            ▒▒▓▓▓▓▓▓▓▓▓▓▓▓"
	@echo "       ▒▒████████▒▒            ░░░░░░░░░░          ██▓▓▓▓██        ░░░░░░░░░░░░          ▓▓▓▓▓▓████▓▓▓▓"
	@echo "         ████████                ▒▒▒▒▒▒            ▓▓██████        ░░░░░░░░░░░░░░░░      ░░▓▓▓▓██▓▓▓▓▒▒              ░░"
	@echo "         ████████                ░░░░░░            ▒▒████▓▓        ░░░░░░░░░░░░░░░░        ██████████            ░░░░░░░░"
	@echo "         ██▓▓████                ░░░░░░            ░░▓▓██▓▓        ░░░░░░░░░░░░░░░░░░      ██▓▓██████            ░░░░░░░░░░"
	@echo "         ██▓▓████                ░░░░░░            ▓▓▓▓▓▓▓▓        ░░░░░░░░░░    ░░░░      ██████████            ░░░░░░░░░░"
	@echo "         ██▓▓▓▓██                ░░░░░░            ░░████▒▒        ░░░░░░░░░░░░            ██████████            ░░▒▒░░░░░░"
	@echo "         ██▓▓▓▓▓▓                ░░░░░░              ██████          ░░░░░░░░░░░░          ██████████            ░░░░░░░░░░"
	@echo "       ░░▓▓▓▓▓▓▓▓                ░░░░░░              ████            ░░░░░░░░░░░░░░        ██████████              ▒▒▒▒▒▒"
	@echo "       ██▓▓▓▓▓▓▓▓                ░░░░░░░░            ▓▓▓▓            ░░░░░░░░░░░░░░        ██████████              ░░░░░░"
	@echo "       ██▓▓▓▓▓▓▓▓              ░░░░░░░░░░            ▓▓██▓▓        ░░░░░░░░░░░░░░░░        ██████████              ░░░░░░"
	@echo "       ██▓▓▓▓▓▓▓▓██            ░░░░░░░░░░          ░░▓▓▓▓▓▓          ░░░░░░░░░░░░░░      ░░██▓▓██▓▓██░░          ░░░░░░░░"
	@echo "     ▓▓▓▓▓▓▓▓▓▓▓▓██▒▒        ░░░░░░░░░░░░░░        ▓▓▓▓▓▓▓▓▒▒          ░░░░░░░░░░        ▓▓▓▓▓▓██▓▓▓▓██          ░░░░░░░░░░"
	@echo " ▓▓██▓▓██████████▓▓▓▓▓▓    ░░░░░░░░░░░░░░░░░░    ░░██▓▓████▓▓▒▒      ░░░░░░░░░░░░░░    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▒▒      ░░░░░░░░░░░░░░"
	@echo " ██████████████████████░░  ░░░░░░░░░░░░░░░░▒▒    ██▓▓▓▓██▓▓▓▓██    ░░░░░░▒▒▒▒▒▒▒▒░░    ████████████▓▓██▓▓    ▒▒░░░░░░░░░░░░░░░░"
	@echo "▒████▓▓▓▓▓▓████▓▓▓▓████▒▒░░▒▒░░░░░░░░░░░░░░▒▒░░░░██▓▓▓▓██▓▓██▓▓▒▒░░▒▒░░░░░░░░░░░░░░░░░░██▓▓▓▓▓▓▓▓████▓▓▓▓░░░░▒▒░░░░░░░░░░░░░░░░░░"

%.o : %.c
# @mkdir -p $(OBJ_DIR)
			$(CC) $(CFLAGS) -I $(INC_DIR) -I $(LIBFT_DIR) $(RL_INC) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJS)
			@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/$(LIBFT) $(RL_LINK) -o $@

$(LIBFT):
			@make -C $(LIBFT_DIR) all
			@make -C $(LIBFT_DIR) bonus

clean:
		@make -C $(LIBFT_DIR) fclean
	$(RM) ./*/*.o
# $(RM) ./src/*.o
# $(RM) ./parsing/*.o
# $(RM) ./builtin/*.o


# @make -C $(GNL_DIR) fclean
			@$(RM) $(OBJ_DIR)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re

mem:
	valgrind -s --track-fds=yes --tool=memcheck --leak-check=full --show-leak-kinds=all --suppressions=.ignore_readline ./minishell