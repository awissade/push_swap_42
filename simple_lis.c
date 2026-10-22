/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_lis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 13:35:26 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 13:44:41 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: lis_update
 * =========================================================================
 * OVERVIEW:
 *  Dynamic programming transition for Longest Increasing Subsequence (LIS)
 *  ending at index 'i'. Initializes the length to 1 and back-pointer to -1,
 *  then scans preceding elements (0 to i - 1) to find the longest chain
 *  that can precede val[i].
 *
 * EXECUTION SCHEMA:
 *
 *   Index i: val[i]
 *        │
 *        ▼
 *   len[i] = 1, prev[i] = -1
 *        │
 *        ↺ Loop j from 0 to i - 1:
 *             │
 *             ├── (val[j] < val[i] && len[j] + 1 > len[i])?
 *             │         ├── [Yes] ──> len[i] = len[j] + 1
 *             │         │             prev[i] = j
 *             │         └── [No]  ──> Continue
 *             └── j++
 *
 * PARAMETERS:
 *  - val {int *}: Array of stack A values in initial order.
 *  - len {int *}: DP table storing LIS lengths ending at each index.
 *  - prev {int *}: Predecessor index tracker for backtracking.
 *  - i {int}: Current target index being evaluated.
 *
 * RETURNS:
 *  - void: Updates len[i] and prev[i] in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(i) comparisons across preceding elements.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - When i == 0, the inner loop is skipped, leaving len[0] = 1.
 *  - Handles strictly increasing sequences (val[j] < val[i]).
 * =========================================================================
 */
static void	lis_update(int *val, int *len, int *prev, int i)
{
	int	j;

	len[i] = 1;
	prev[i] = -1;
	j = 0;
	while (j < i)
	{
		if (val[j] < val[i] && len[j] + 1 > len[i])
		{
			len[i] = len[j] + 1;
			prev[i] = j;
		}
		j++;
	}
}

/**
 * =========================================================================
 * FUNCTION: lis_best
 * =========================================================================
 * OVERVIEW:
 *  Computes the full dynamic programming table for the LIS across all 'n'
 *  elements by invoking lis_update sequentially. Tracks and returns the
 *  index of the terminal element of the longest chain found.
 *
 * EXECUTION SCHEMA:
 *
 *   best = 0
 *        │
 *        ↺ Loop i from 0 to n - 1:
 *             │
 *             ├── lis_update(val, len, prev, i)
 *             └── len[i] > len[best]?
 *                       ├── [Yes] ──> best = i
 *                       └── [No]  ──> Continue
 *        │
 *        ▼
 *   Return best (index of LIS terminal element)
 *
 * PARAMETERS:
 *  - val {int *}: Array of values from stack A.
 *  - len {int *}: DP table recording longest lengths.
 *  - prev {int *}: Array storing backtrack links.
 *  - n {int}: Total element count in the array.
 *
 * RETURNS:
 *  - {int}: Zero-based index of the element where the maximal LIS ends.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N^2) quadratic DP sweep.
 *  - Space Complexity: O(1) auxiliary space beyond input buffers.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - In case of ties, preserves the earliest occurrence of maximum length.
 *  - Requires n >= 1.
 * =========================================================================
 */
static int	lis_best(int *val, int *len, int *prev, int n)
{
	int	i;
	int	best;

	best = 0;
	i = 0;
	while (i < n)
	{
		lis_update(val, len, prev, i);
		if (len[i] > len[best])
			best = i;
		i++;
	}
	return (best);
}

/**
 * =========================================================================
 * FUNCTION: lis_mark
 * =========================================================================
 * OVERVIEW:
 *  Extracts stack A into a linear array, determines the optimal LIS, and
 *  backtracks through predecessor indices to flag elements belonging to the
 *  subsequence in the 'keep' mask array (1 = keep in A, 0 = move to B).
 *
 * EXECUTION SCHEMA:
 *
 *   Extract stack A nodes into val[0..size-1]
 *             │
 *             ▼
 *   best = lis_best(val, len, prev, size)
 *             │
 *             ▼
 *   Backtrack via prev links:
 *   i = best
 *   while (i >= 0):
 *        keep[i] = 1
 *        i = prev[i]
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stack A and size.
 *  - keep {int *}: Destination mask array initialized to zeros.
 *
 * RETURNS:
 *  - void: Populates the keep mask with 1s for LIS elements.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N^2) dominated by LIS calculation.
 *  - Space Complexity: O(N) tracked heap memory for val, len, and prev.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Assumes keep array has been zero-initialized by the caller.
 *  - Allocates scratch buffers via s_alloc; automatically reclaimed on exit.
 * =========================================================================
 */
static void	lis_mark(t_config *config, int *keep)
{
	int		*val;
	int		*len;
	int		*prev;
	int		i;
	t_stack	*tmp;

	val = s_alloc(config->nums.size * sizeof(int));
	len = s_alloc(config->nums.size * sizeof(int));
	prev = s_alloc(config->nums.size * sizeof(int));
	tmp = config->nums.a;
	i = 0;
	while (i < config->nums.size)
	{
		val[i++] = tmp->value;
		tmp = tmp->next;
	}
	i = lis_best(val, len, prev, config->nums.size);
	while (i >= 0)
	{
		keep[i] = 1;
		i = prev[i];
	}
}

/**
 * =========================================================================
 * FUNCTION: push_others
 * =========================================================================
 * OVERVIEW:
 *  Initial partition phase for the simple strategy. Identifies the Longest
 *  Increasing Subsequence in stack A and pushes all non-LIS elements into
 *  stack B via pb, retaining LIS elements in stack A using ra rotations.
 *
 * EXECUTION SCHEMA:
 *
 *   1. keep = s_alloc(size * sizeof(int))
 *   2. memset(keep, 0, size * sizeof(int))
 *   3. lis_mark(config, keep)
 *             │
 *             ▼
 *   Loop i from 0 to size - 1:
 *        ├── keep[i] == 1 ──> ra(config)  [Keep LIS element in A]
 *        └── keep[i] == 0 ──> pb(config)  [Push remaining to B]
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Modifies stacks A and B, leaving only LIS nodes inside stack A.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N^2) due to LIS identification, plus O(N) moves.
 *  - Space Complexity: O(N) tracked heap memory for the keep mask.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Exactly (size - LIS_len) elements are transferred to stack B.
 *  - Stack A order of LIS elements is preserved circularly.
 * =========================================================================
 */
void	push_others(t_config *config)
{
	int	*keep;
	int	i;

	keep = s_alloc(config->nums.size * sizeof(int));
	ft_memset(keep, 0, config->nums.size * sizeof(int));
	lis_mark(config, keep);
	i = 0;
	while (i < config->nums.size)
	{
		if (keep[i])
			ra(config);
		else
			pb(config);
		i++;
	}
}
