/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:24:47 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/23 15:39:16 by nychan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: str_to_int
 * =========================================================================
 * OVERVIEW:
 *  Strict ASCII-to-integer parser. Validates single optional sign prefix,
 *  rejects non-digit characters, checks for 32-bit signed integer overflow
 *  or underflow, and writes the converted integer to the destination pointer.
 *
 * EXECUTION SCHEMA:
 *
 *   str (string)
 *        │
 *        ▼
 *   Optional Sign (+/-)
 *        │
 *        ▼
 *   *str == '\0'? ───[Yes]───> Return false (lone sign or empty)
 *        │ [No]
 *        ▼
 *   Digit Loop (*str):
 *        ├── Non-digit (not '0'-'9') ──> Return false
 *        ├── total = total * 10 + digit
 *        └── (total * sign) out of [INT_MIN, INT_MAX]?
 *                 ├── [Yes] ───────────> Return false (Overflow)
 *                 └── [No]  ───────────> Next character
 *        │
 *        ▼ (End of string)
 *   *result = (int)(total * sign)
 *   Return true
 *
 * PARAMETERS:
 *  - str {char *}: Null-terminated numeric string to parse.
 *  - result {int *}: Memory address to store the resulting integer.
 *
 * RETURNS:
 *  - {bool}: true on successful conversion within [INT_MIN, INT_MAX];
 *            false on non-numeric characters, lone signs, or overflow.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(D), where D is the character length of the string.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Rejects embedded characters or letters (e.g., "-123a4").
 *  - Rejects lone sign tokens (e.g., "+" or "-") and empty strings.
 *  - Validates full 32-bit signed range using a long accumulator.
 * =========================================================================
 */
bool	str_to_int(char *str, int *result)
{
	int		sign;
	long	total;

	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (false);
	total = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		total = (total * 10) + (*str - '0');
		if ((total * sign) > INT_MAX || (total * sign) < INT_MIN)
			return (false);
		str++;
	}
	*result = (int)(total * sign);
	return (true);
}

/**
 * =========================================================================
 * FUNCTION: set_strategy
 * =========================================================================
 * OVERVIEW:
 *  Assigns the sorting strategy in the program configuration. Enforces
 *  exclusivity by ensuring a strategy is only accepted if none has been
 *  configured yet.
 *
 * EXECUTION SCHEMA:
 *
 *   config->strat != NONE?
 *         │
 *         ├─── Yes ───> Return false (Strategy already assigned)
 *         │
 *         └─── No  ───> config->strat = start
 *                       Return true
 *
 * PARAMETERS:
 *  - config {t_config *}: Target program configuration struct.
 *  - start {t_strat}: Strategy enum value to assign.
 *
 * RETURNS:
 *  - {bool}: true if assignment succeeds; false if a strategy was already set.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Prevents configuration conflicts from multiple conflicting CLI flags.
 * =========================================================================
 */
static bool	set_strategy(t_config *config, t_strat start)
{
	if (config->strat != NONE)
		return (false);
	config->strat = start;
	return (true);
}

/**
 * =========================================================================
 * FUNCTION: parse_flag
 * =========================================================================
 * OVERVIEW:
 *  Parses command-line option flags. Validates and marks the benchmark mode
 *  or delegates strategy assignment, rejecting unknown or duplicate flags.
 *
 * EXECUTION SCHEMA:
 *
 *   str (flag token)
 *        │
 *        ├── "--bench"? ──> config->bench != 0?
 *        │                    ├── Yes ──> Return false (Duplicate)
 *        │                    └── No  ──> config->bench = 1, Return true
 *        │
 *        ├── "--simple"   ──> set_strategy(config, SIMPLE)
 *        ├── "--medium"   ──> set_strategy(config, MEDIUM)
 *        ├── "--complex"  ──> set_strategy(config, COMPLEX)
 *        ├── "--adaptive" ──> set_strategy(config, ADAPTIVE)
 *        │
 *        └── [Unrecognized] ──> Return false
 *
 * PARAMETERS:
 *  - str {char *}: Null-terminated argument flag token.
 *  - config {t_config *}: Configuration struct to update.
 *
 * RETURNS:
 *  - {bool}: true if a valid, unrepeated flag is registered;
 *            false on unrecognized flags or duplicate flag declarations.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1) bounded string comparison.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Fails if '--bench' is supplied more than once.
 *  - Fails if multiple strategy flags are provided in the same invocation.
 * =========================================================================
 */
bool	parse_flag(char *str, t_config *config)
{
	if (!ft_strncmp(str, "--bench", 8))
	{
		if (config->bench)
			return (false);
		config->bench = 1;
		return (true);
	}
	else if (!ft_strncmp(str, "--simple", 9))
		return (set_strategy(config, SIMPLE));
	else if (!ft_strncmp(str, "--medium", 9))
		return (set_strategy(config, MEDIUM));
	else if (!ft_strncmp(str, "--complex", 10))
		return (set_strategy(config, COMPLEX));
	else if (!ft_strncmp(str, "--adaptive", 11))
		return (set_strategy(config, ADAPTIVE));
	return (false);
}
