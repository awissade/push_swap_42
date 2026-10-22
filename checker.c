/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 14:21:30 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/06 16:36:57 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker.h"

/**
 * =========================================================================
 * FUNCTION: do_operation
 * =========================================================================
 * OVERVIEW:
 *  Command dispatcher for checker operations. Matches an input line against
 *  known push_swap instructions, executes the matching operation function,
 *  and indicates whether a recognized command was handled.
 *
 * EXECUTION SCHEMA:
 *
 *   line (string from stdin)
 *        │
 *        ├── "pa\n"  ──> pa(config)
 *        ├── "pb\n"  ──> pb(config)
 *        ├── "sa\n"  ──> sa(config)
 *        ├── "sb\n"  ──> sb(config)
 *        ├── "ss\n"  ──> ss(config)
 *        ├── "ra\n"  ──> ra(config)
 *        ├── "rb\n"  ──> rb(config)
 *        ├── "rr\n"  ──> rr(config)
 *        ├── "rra\n" ──> rra(config)
 *        ├── "rrb\n" ──> rrb(config)
 *        ├── "rrr\n" ──> rrr(config)
 *        │
 *        ├── [Matched] ───────────────> Return 1
 *        └── [Unrecognized] ──────────> Return 0
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and settings.
 *  - line {char *}: Null-terminated line read from standard input.
 *
 * RETURNS:
 *  - {int}: 1 if the line matches a valid operation and was executed;
 *           0 if the line does not match any valid instruction.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1) bounded string prefix comparisons.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Expects trailing newline ('\n') on all input instructions.
 *  - Compares exact byte lengths including '\n' to prevent partial matches.
 * =========================================================================
 */
int	do_operation(t_config *config, char *line)
{
	if (!ft_strncmp(line, "pa\n", 4))
		pa(config);
	else if (!ft_strncmp(line, "pb\n", 4))
		pb(config);
	else if (!ft_strncmp(line, "sa\n", 4))
		sa(config);
	else if (!ft_strncmp(line, "sb\n", 4))
		sb(config);
	else if (!ft_strncmp(line, "ss\n", 4))
		ss(config);
	else if (!ft_strncmp(line, "ra\n", 4))
		ra(config);
	else if (!ft_strncmp(line, "rb\n", 4))
		rb(config);
	else if (!ft_strncmp(line, "rr\n", 4))
		rr(config);
	else if (!ft_strncmp(line, "rra\n", 5))
		rra(config);
	else if (!ft_strncmp(line, "rrb\n", 5))
		rrb(config);
	else if (!ft_strncmp(line, "rrr\n", 5))
		rrr(config);
	else
		return (0);
	return (1);
}

/**
 * =========================================================================
 * FUNCTION: read_and_do
 * =========================================================================
 * OVERVIEW:
 *  Reads instructions sequentially from standard input (fd 0) until EOF.
 *  Passes each line to do_operation; if an instruction is invalid, prints
 *  "Error\n" to standard error (fd 2), purges allocated memory, and exits.
 *
 * EXECUTION SCHEMA:
 *
 *   Loop (get_next_line(0)):
 *        │
 *        ├── line == NULL? ──────[Yes]──> Return (EOF reached cleanly)
 *        │
 *        ▼ [Valid line]
 *   do_operation(config, line)
 *        │
 *        ├── [0 / Unrecognized] ───────> ft_printf("Error\n", 2)
 *        │                               g_free()
 *        │                               exit(1)
 *        │
 *        └── [1 / Recognized]   ───────> Repeat loop
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks to mutate.
 *
 * RETURNS:
 *  - void: Returns normally on clean EOF; terminates process on error.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(M), where M is the total count of instruction lines.
 *  - Space Complexity: O(1) auxiliary space per line beyond GNL buffers.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Empty stream (immediate EOF) returns cleanly without doing any work.
 *  - Immediate error handling: aborts on first invalid line without continuing.
 * =========================================================================
 */
void	read_and_do(t_config *config)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = get_next_line(0);
		if (!line)
			return ;
		if (!do_operation(config, line))
		{
			ft_printf("Error\n", 2);
			exit ((g_free(), 1));
		}
	}
}

/**
 * =========================================================================
 * FUNCTION: check_if_sorted
 * =========================================================================
 * OVERVIEW:
 *  Validates whether stack A is fully sorted in ascending order and stack B
 *  is completely empty. Prints "OK\n" if valid, or prints "KO\n" and exits
 *  with status 1 if any order violation or leftover element is detected.
 *
 * EXECUTION SCHEMA:
 *
 *   tmp = nums.a->prev (tail node)
 *        │
 *        ▼
 *   Loop while (nums.a != tmp):
 *        │
 *        ├── (a->value > a->next->value) || nums.b != NULL?
 *        │         ├── [Yes] ──> ft_printf("KO\n", 1)
 *        │         │             g_free()
 *        │         │             exit(1)
 *        │         │
 *        │         └── [No]  ──> nums.a = nums.a->next
 *        │
 *        ▼ (Traversed entire ring)
 *   ft_printf("OK\n", 1)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks A and B.
 *
 * RETURNS:
 *  - void: Outputs "OK\n" on success; exits process with status 1 on "KO\n".
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the total node count in stack A.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Traversal loops up to tail node 'tmp' to avoid infinite circular cycles.
 *  - Fails immediately if stack B retains any elements after execution.
 * =========================================================================
 */
void	check_if_sorted(t_config *config)
{
	t_stack	*tmp;

	tmp = config->nums.a->prev;
	while (config->nums.a != tmp)
	{
		if (config->nums.a->value > config->nums.a->next->value
			|| config->nums.b)
		{
			ft_printf("KO\n", 1);
			exit ((g_free(), 1));
		}
		config->nums.a = config->nums.a->next;
	}
	ft_printf("OK\n", 1);
}

/**
 * =========================================================================
 * FUNCTION: main
 * =========================================================================
 * OVERVIEW:
 *  Entry point for the checker program. Validates arguments, initializes
 *  configuration, parses input values into stack A, enables silent mode to
 *  suppress operation logging, applies instructions from stdin, verifies
 *  final stack order, and frees all allocated memory before exit.
 *
 * EXECUTION SCHEMA:
 *
 *   ac < 2? ─────────────────────[Yes]──> Return 1 (Silent exit)
 *        │ [No]
 *        ▼
 *   ft_memset(&config, 0)
 *   parse_input(ac, av, &config)
 *        ├── [Failed] ──────────────────> ft_printf("Error\n", 2)
 *        │                                g_free()
 *        │                                Return 1
 *        ▼ [Success]
 *   config.silent = 1 (Suppress operation prints)
 *   read_and_do(&config)
 *   check_if_sorted(&config)
 *   g_free()
 *   Return 0
 *
 * PARAMETERS:
 *  - ac {int}: Command-line argument count.
 *  - av {char **}: Command-line argument vector.
 *
 * RETURNS:
 *  - {int}: 0 on successful sorted verification; 1 on parsing or input error.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(L + N^2 + M), where L is argument length, N is
 *                      the number count, and M is the operation count.
 *  - Space Complexity: O(L + N) tracked heap memory for numbers and stacks.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Silently exits with 1 if no parameters are passed (ac < 2).
 *  - Reclaims all garbage-collector tracked memory through g_free() on exit.
 * =========================================================================
 */
int	main(int ac, char **av)
{
	t_config	config;
	bool		is_flag;

	if (ac < 2)
		return (1);
	ft_memset(&config, 0, sizeof(t_config));
	is_flag = (av[1][0] == '-' && av[1][1] == '-');
	if (is_flag || !parse_input(ac, av, &config))
	{
		ft_printf("Error\n", 2);
		g_free();
		return (1);
	}
	config.silent = 1;
	read_and_do(&config);
	check_if_sorted(&config);
	g_free();
	return (0);
}
