# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/19 13:35:34 by jesuserr          #+#    #+#              #
#    Updated: 2023/06/16 12:59:44 by jesuserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_DIR = libft/

NAME = fdf
SRCS = main.c map_utils.c errors.c z_utils.c rotations.c projections.c \
graphics.c moves.c hooks.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

INCLUDE = -I./ -I./libft/includes/
RM = rm -f
CFLAGS = -O3 -Wall -Wextra -Werror
LDFLAGS = -lSDL2 -lSDL2_gfx -lm

NORM = ${SRCS} fdf.h
GREEN = "\033[0;92m"
RED = "\033[0;91m"
BLUE = "\033[0;94m"
NC = "\033[37m"

all: makelibft $(NAME)

makelibft:
	@make --no-print-directory -C $(LIBFT_DIR)	
	@echo ${GREEN}"Libft Compiled!\n"${NC};

%.o: %.c
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@
	
$(NAME): $(OBJS) $(LIBFT_DIR)libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)libft.a -o $@ $(LDFLAGS)
	@echo ${GREEN}"fdf Compiled!\n"${NC};
-include $(DEPS)

clean:
	@make clean --no-print-directory -C $(LIBFT_DIR)
	$(RM) $(OBJS) $(DEPS)
		
fclean:
	@make fclean --no-print-directory -C $(LIBFT_DIR)
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

norm:
	@echo ${BLUE}"\nChecking Norminette..."${NC}
	@if norminette $(NORM); then echo ${GREEN}"Norminette OK!\n"${NC}; \
	else echo ${RED}"Norminette KO!\n"${NC}; \
	fi

re: fclean all

.PHONY: all clean fclean re makelibft norm
