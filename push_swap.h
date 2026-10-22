/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:34:06 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 14:34:10 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "./libft/libft.h"
# include <stdbool.h>
# include <limits.h>

typedef struct s_stack
{
	int				value;
	struct s_stack	*next;
	struct s_stack	*prev;
}	t_stack;

typedef struct s_move
{
	int	x;
	int	y;
	int	cost;
}	t_move;

typedef struct s_nums
{
	t_stack	*a;
	t_stack	*b;
	int		*arr;
	int		*tmp;
	int		size;
	int		size_a;
	int		size_b;
}	t_nums;

typedef enum e_strat
{
	NONE,
	SIMPLE,
	MEDIUM,
	COMPLEX,
	ADAPTIVE
}	t_strat;

typedef struct s_bench
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
	int	total;
}	t_bench;

typedef struct s_select
{
	int		lowest;
	int		highest;
	int		mins;
	bool	pend;
}	t_select;

typedef struct s_config
{
	t_strat	strat;
	t_strat	chosen;
	int		bench;
	t_nums	nums;
	t_bench	metrics;
	bool	silent;
	float	disorder;
	int		pivot;
	int		ka;
	int		kb;
}	t_config;

// those functions for the garbage collector system
// switch to the file to read more about each function
t_list		**get_head(void);
void		*s_alloc(size_t size);
void		g_free(void);
// those function is for puting numbers in the stack a after the parsing
// switch to the file to read more about each function
t_stack		*lst_new(int value);
void		lstadd_back(t_stack **a, t_stack *new);
// those function are used in the parsing
// switch to the file to read more about each function
bool		parse_input(int ac, char **av, t_config *config);
bool		add_number(char *arr, t_config *config);
char		*join_all_args(int ac, char **av);
bool		parse_flag(char *str, t_config *config);
bool		str_to_int(char *str, int *result);
char		**ft_split_ad(char const *s);
// those function is for the operations that we should apply for the stacks
// switch to the file to read more about each function
void		pa(t_config *config);
void		pb(t_config *config);
void		sa(t_config *config);
void		sb(t_config *config);
void		ss(t_config *config);
void		ra(t_config *config);
void		rb(t_config *config);
void		rr(t_config *config);
void		rra(t_config *config);
void		rrb(t_config *config);
void		rrr(t_config *config);

// this is for sort the array
long long	merge_sort(t_nums *nums);
float		calculate_disorder(t_config *config);
void		calculate_pivot(t_config *config);

void		medium(t_config *config);
void		complex(t_config *config);
void		simple(t_config *c);
void		adaptive(t_config *config);

void		bench(t_config *config);

void		sort_two(t_config *config);
void		sort_three(t_config *config);
void		sort_a(t_config *config, int *arr, int size);
void		sort_b(t_config *config, int *arr, int size);
void		median_partation_a(t_config *config, int *arr, int size,
				int *pb_count);
void		median_partation_b(t_config *config, int *arr, int size,
				int *pa_count);
void		select_b(t_config *c, int *arr, int size);

void		push_others(t_config *config);
void		best_way(t_config *config, int af, int bf, t_move *move);
void		rotate_pair(t_config *config, int x, int y);
void		rotate_a(t_config *config, int steps);
int			is_sorted_a(t_stack *head);
#endif
