/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adaptive.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 13:29:55 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 13:30:31 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: adaptive
 * =========================================================================
 * OVERVIEW:
 *  Adaptive strategy dispatcher. Inspects the precomputed disorder metric
 *  (normalized inversion ratio) and delegates execution to the most
 *  mechanically efficient sorting algorithm for that entropy level.
 *
 * EXECUTION SCHEMA:
 *
 *   config->disorder
 *         │
 *         ├── < 0.2 ───────> simple(config)   [LIS + Turk Sort: O(n^2)]
 *         │
 *         ├── < 0.5 ───────> medium(config)   [Chunk / Window: O(n√n)]
 *         │
 *         └── >= 0.5 ──────> complex(config)  [Quicksort: O(n log n)]
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing the computed disorder
 *                         score and stack structures.
 *
 * RETURNS:
 *  - void: Modifies stacks in place until stack A is sorted.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(n^2) for Simple, O(n√n) for Medium, or
 *                      O(n log n) for Complex.
 *  - Space Complexity: Matches the auxiliary allocations of the chosen
 *                      sorting routine.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Assumes calculate_disorder has already been called to evaluate the
 *    permutation entropy and populate config->disorder.
 *  - Boundary thresholds (< 0.2, < 0.5, >= 0.5) guarantee total coverage
 *    for all possible valid disorder values.
 * =========================================================================
 */
void	adaptive(t_config *config)
{
	if (config->disorder < 0.2)
		simple(config);
	else if (config->disorder < 0.5)
		medium(config);
	else
		complex(config);
}
