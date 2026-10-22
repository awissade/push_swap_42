# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abwissad <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/17 14:33:04 by abwissad          #+#    #+#              #
#    Updated: 2026/10/17 14:33:05 by abwissad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=		push_swap
B_NAME		=		checker

LIBFT_DIR	=		libft
LIBFT		=		$(LIBFT_DIR)/libft.a

CC			=		cc
CFLAGS		=		-Wextra -Wall -Werror -I$(LIBFT_DIR) -g3
RM			=		rm -f

COMMON_SRCS	=		ft_split_ad.c gc.c lst_new.c lstadd_back.c parse_flags.c	\
					parse_input.c push_operations.c swap_operations.c			\
					rotate_operations.c r_rotate_operations.c

SRCS		=		main.c $(COMMON_SRCS)										\
					disorder.c medium.c complex.c complex_partation.c pivot.c	\
					sort_or_reverse.c	simple.c simple_lis.c simple_move.c		\
					adaptive.c bench.c complex_select.c

B_SRCS		=		checker.c $(COMMON_SRCS) 									\
					get_next_line.c get_next_line_utils.c


OBJS		=		$(SRCS:.c=.o)

B_OBJS		=		$(B_SRCS:.c=.o)

all			:		$(NAME)

bonus		:		$(B_NAME)

$(NAME)		:		$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(B_NAME)	:		$(B_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(B_OBJS) $(LIBFT) -o $(B_NAME)

%.o			:		%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT)	:
	@make -C $(LIBFT_DIR)

clean		:
	$(RM) $(OBJS) $(B_OBJS)

fclean		:		clean
	@make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME) $(B_NAME)

re			:		fclean all

rb			:		fclean bonus

.PHONY		:		all clean fclean re rb
