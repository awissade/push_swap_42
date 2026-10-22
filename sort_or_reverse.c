/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_or_reverse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 09:25:23 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 13:48:11 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: sort_two
 * =========================================================================
 * OVERVIEW:
 *  Base-case sorter for a two-element segment at the top of stack A.
 *  Compares the top two values and executes 'sa' if they are inverted.
 *
 * EXECUTION SCHEMA:
 *
 *   nums.a->value > nums.a->next->value?
 *         │
 *         ├── [Yes] ──> sa(config)
 *         │
 *         └── [No]  ──> Return (Already sorted)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stack A and metrics.
 *
 * RETURNS:
 *  - void: Mutates top two elements of stack A in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1) at most one 'sa' instruction.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Assumes stack A contains at least 2 valid nodes.
 *  - No-op if elements are already in ascending order.
 * =========================================================================
 */
void	sort_two(t_config *config)
{
	if (config->nums.a->value > config->nums.a->next->value)
		sa(config);
}

/**
 * =========================================================================
 * FUNCTION: sort_three
 * =========================================================================
 * OVERVIEW:
 *  Sorts the top three elements of stack A in ascending order without using
 *  rotations (ra/rra). Uses 'sa' and temporary staging into stack B (pb ->
 *  sa -> pa), preserving the positioning of any elements beneath the top
 *  three in recursive sub-slices.
 *
 * EXECUTION SCHEMA:
 *
 *   Read: one = a[0], two = a[1], three = a[2]
 *     │
 *     ├── one < two && two < three? ──[Yes]──> Return (Sorted)
 *     │
 *     ├── two < one?
 *     │     ├── sa(config)
 *     │     └── sort_three(config) ──────────> Recurse
 *     │
 *     └── Otherwise (one < two && two >= three):
 *           ├── pb(config)
 *           ├── sa(config)
 *           ├── pa(config)
 *           └── sort_three(config) ──────────> Recurse
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks A and B.
 *
 * RETURNS:
 *  - void: Modifies the top three elements of stack A in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1) bounded recursion (at most 2-3 passes).
 *  - Space Complexity: O(1) bounded call stack frames.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Does not disrupt nodes deeper in stack A because no ra/rra is used.
 *  - Temporarily uses top of stack B as a 1-element scratchpad.
 * =========================================================================
 */
void	sort_three(t_config *config)
{
	int	one;
	int	two;
	int	three;

	one = config->nums.a->value;
	two = config->nums.a->next->value;
	three = config->nums.a->next->next->value;
	if (one < two && two < three)
		return ;
	else if (two < one)
	{
		sa(config);
		sort_three(config);
	}
	else
	{
		pb(config);
		sa(config);
		pa(config);
		sort_three(config);
	}
}
