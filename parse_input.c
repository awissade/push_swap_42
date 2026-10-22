/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:24:21 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/23 15:39:16 by nychan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: join_all_args
 * =========================================================================
 * OVERVIEW:
 *  Aggregates command-line arguments (av[1] through av[ac - 1]) into a single
 *  space-delimited string buffer tracked by the garbage collector.
 *
 * EXECUTION SCHEMA:
 *
 *   av[1]   av[2]   ...   av[ac-1]
 *     │       │              │
 *     └───────┴──────┬───────┘
 *                    ▼
 *            1. Calculate Total Length
 *                    ▼
 *            2. s_alloc(total_len + spaces + 1)
 *                    │
 *             ┌──────┴──────┐
 *          [NULL]        [Valid]
 *             │             │
 *       g_free() + exit     ▼
 *                    3. Copy strings with ' ' delimiter
 *                    4. Append '\0' terminator
 *                    5. Return joined line
 *
 * PARAMETERS:
 *  - ac {int}: Total count of command-line arguments.
 *  - av {char **}: Vector of null-terminated argument strings.
 *
 * RETURNS:
 *  - {char *}: Pointer to the newly allocated concatenated string.
 *  - Exits process with code 1 if memory allocation fails.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(L), where L is the total length of all arguments.
 *  - Space Complexity: O(L) tracked heap memory for the merged buffer.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Requires ac >= 2; assumes av contains non-null strings up to index ac.
 *  - Critical Failure: Triggers immediate g_free() and aborts if s_alloc fails.
 * =========================================================================
 */
char	*join_all_args(int ac, char **av)
{
	int		i;
	int		j;
	size_t	total_len;
	char	*line;

	i = 0;
	total_len = 0;
	while (av[++i])
		total_len += ft_strlen(av[i]);
	line = s_alloc(((total_len + (ac - 2) + 1) * sizeof(char)));
	if (!line)
		exit ((g_free(), 1));
	i = 1;
	j = 0;
	while (av[i])
	{
		total_len = 0;
		while (av[i][total_len])
			line[j++] = av[i][total_len++];
		if (i < ac - 1)
			line[j++] = ' ';
		i++;
	}
	line[j] = '\0';
	return (line);
}

/**
 * =========================================================================
 * FUNCTION: add_number
 * =========================================================================
 * OVERVIEW:
 *  Parses a numeric token string into an integer, validates against duplicate
 *  values already present, and records it into both the contiguous tracking
 *  array and the linked list stack.
 *
 * EXECUTION SCHEMA:
 *
 *   arr (numeric string)
 *           │
 *           ▼
 *      str_to_int() ──[Error/Overflow]──> Return false
 *           │
 *      [Valid int]
 *           │
 *           ▼
 *   Duplicate Check (nums.arr[0..size-1] == number?)
 *           ├─ Yes ─────────────────────> Return false
 *           │
 *           ▼ (No duplicate)
 *   Append to nums.arr & increment size
 *           │
 *           ▼
 *   lst_new(number) ──[Alloc fail]──────> Return false
 *           │
 *      [Valid node]
 *           │
 *           ▼
 *   lstadd_back(&nums.a, node)
 *   Return true
 *
 * PARAMETERS:
 *  - arr {char *}: Null-terminated numeric string to convert.
 *  - config {t_config *}: Target runtime configuration and stack state.
 *
 * RETURNS:
 *  - {bool}: true on successful conversion, duplicate check, and enqueue;
 *            false on conversion failure, duplicate found, or malloc error.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(K), where K is current size (linear duplicate check).
 *  - Space Complexity: O(1) auxiliary per insertion.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Rejects integer overflow/underflow or non-numeric tokens via str_to_int.
 *  - Rejects duplicate values to maintain permutation uniqueness.
 * =========================================================================
 */
bool	add_number(char *arr, t_config *config)
{
	int		number;
	t_stack	*node;
	int		j;

	if (!str_to_int(arr, &number))
		return (false);
	j = 0;
	while (j < config->nums.size)
	{
		if (config->nums.arr[j] == number)
			return (false);
		j++;
	}
	config->nums.arr[config->nums.size] = number;
	config->nums.size++;
	node = lst_new(number);
	if (!node)
		return (false);
	lstadd_back(&config->nums.a, node);
	return (true);
}

/**
 * =========================================================================
 * FUNCTION: process_all_tokens
 * =========================================================================
 * OVERVIEW:
 *  Sequentially classifies and routes split tokens into option flags or stack
 *  integers. Enforces strict positional ordering where configuration flags
 *  must precede all numerical input values.
 *
 * EXECUTION SCHEMA:
 *
 *    Tokens: [ arr[0], arr[1], ..., arr[n] ]
 *                     │
 *                     ▼
 *        Does arr[i] start with "--"?
 *         ├─── Yes ───> num_start == true?
 *         │              ├─── Yes ───> Return false (Flag after numbers)
 *         │              └─── No  ───> parse_flag() == false?
 *         │                             ├─── Yes ───> Return false
 *         │                             └─── No  ───> Continue
 *         │
 *         └─── No  ───> num_start = true
 *                       add_number() == false?
 *                        ├─── Yes ───> Return false
 *                        └─── No  ───> Continue
 *
 * PARAMETERS:
 *  - arr {char **}: Null-terminated array of split argument tokens.
 *  - config {t_config *}: Program configuration struct being populated.
 *
 * RETURNS:
 *  - {bool}: true if all tokens are valid and ordered; false if a flag is
 *            unknown, placed after a number, or contains an invalid integer.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N * K), where N is token count and K is stack size.
 *  - Space Complexity: O(1) auxiliary stack space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Disallows flags appearing anywhere after the first numeric value.
 *  - Short-circuits immediately upon encountering any invalid token.
 * =========================================================================
 */
static bool	process_all_tokens(char **arr, t_config *config)
{
	bool	num_start;
	int		i;

	num_start = false;
	i = -1;
	while (arr[++i])
	{
		if (arr[i][0] == '-' && arr[i][1] == '-')
		{
			if (!parse_flag(arr[i], config) || num_start)
				return (false);
		}
		else
		{
			num_start = true;
			if (!add_number(arr[i], config))
				return (false);
		}
	}
	return (true);
}

/**
 * =========================================================================
 * FUNCTION: parse_input
 * =========================================================================
 * OVERVIEW:
 *  Main parsing facade. Normalizes CLI inputs by flattening, tokenizing across
 *  whitespaces, preallocating the number buffer, processing all flags and
 *  numeric elements, and falling back to default operational settings.
 *
 * EXECUTION SCHEMA:
 *
 *   CLI Args (ac, av)
 *          │
 *          ▼
 *   join_all_args() ───> Flatten arguments into one string
 *          │
 *          ▼
 *   ft_split_ad()   ───> Split line by whitespace tokens
 *          │
 *          ▼
 *   s_alloc()       ───> Preallocate nums.arr buffer (token count)
 *          │
 *          ▼
 *   process_all_tokens()
 *          │
 *          ├─ Invalid flag / duplicate / NaN ──> Return false
 *          │
 *          ▼
 *   Check & apply default: (strat == NONE) ───> strat = ADAPTIVE
 *          │
 *          ▼
 *   Validate count: (nums.size == 0)       ───> Return false
 *          │
 *          ▼
 *   Return true
 *
 * PARAMETERS:
 *  - ac {int}: Argument count from main().
 *  - av {char **}: Argument vector from main().
 *  - config {t_config *}: State struct to initialize with parsed values.
 *
 * RETURNS:
 *  - {bool}: true if arguments form a valid, non-empty integer sequence;
 *            false on syntax errors, empty sets, duplicate items, or OOM.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(L + N^2), where L is raw string length and N is the
 *                      total number of integer tokens parsed.
 *  - Space Complexity: O(L + N) managed memory for tokens and integer array.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Automatically assigns ADAPTIVE strategy if no flag override was set.
 *  - Rejects inputs containing flags only (nums.size == 0).
 * =========================================================================
 */
bool	parse_input(int ac, char **av, t_config *config)
{
	char	*arg_line;
	char	**arr;
	int		i;

	arg_line = join_all_args(ac, av);
	arr = ft_split_ad(arg_line);
	if (!arr)
		return (false);
	i = 0;
	while (arr[i])
		i++;
	config->nums.arr = s_alloc(i * sizeof(int));
	if (!config->nums.arr)
		return (false);
	if (!process_all_tokens(arr, config))
		return (false);
	if (config->strat == NONE)
		config->strat = ADAPTIVE;
	if (config->nums.size == 0)
		return (false);
	return (true);
}
