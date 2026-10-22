/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_partation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 13:30:47 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 13:30:48 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: calculate_index
 * =========================================================================
 * OVERVIEW:
 *  Calculates a partition split index within a sorted reference array
 *  using a scaled ratio (pivot / 20). Enforces boundary clamping to
 *  prevent extreme pivots from yielding empty sub-partitions.
 *
 * EXECUTION SCHEMA:
 *
 *   idx = (size * pivot) / 20
 *            │
 *            ├── a is true (Stack A Partition):
 *            │     idx > size - 2? ──[Yes]──> idx = size - 2
 *            │
 *            └── a is false (Stack B Partition):
 *                  idx < 1?        ──[Yes]──> idx = 1
 *                  idx > size - 1? ──[Yes]──> idx = size - 1
 *            │
 *            ▼
 *   Return idx
 *
 * PARAMETERS:
 *  - size {int}: Current segment size to be partitioned.
 *  - pivot {int}: Integer scaling factor (out of 20, e.g., 8 to 12).
 *  - a {int}: Flag indicating context (1 for stack A, 0 for stack B).
 *
 * RETURNS:
 *  - {int}: Clamped index within sorted subarray to use as pivot value.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Stack A clamping prevents keeping fewer than 2 elements in A.
 *  - Stack B clamping guarantees at least 1 element is pushed to A.
 * =========================================================================
 */
static int	calculate_index(int size, int pivot, int a)
{
	int	idx;

	idx = size * pivot / 20;
	if (a)
	{
		if (idx > size - 2)
			idx = size - 2;
	}
	else
	{
		if (idx < 1)
			idx = 1;
		if (idx > size - 1)
			idx = size - 1;
	}
	return (idx);
}

/**
 * =========================================================================
 * FUNCTION: median_partation_a
 * =========================================================================
 * OVERVIEW:
 *  Partitions a sub-slice of stack A into stack B using the pivot value
 *  at arr[idx]. Elements <= arr[idx] are pushed to B via pb. Elements
 *  > arr[idx] are rotated down via ra. If partitioning a partial stack,
 *  unwinds rotations via rra to maintain remaining stack ordering.
 *
 * EXECUTION SCHEMA:
 *
 *   idx = calculate_index(size, pivot, 1)
 *   whole = (size_a == size)
 *            │
 *            ▼
 *   Loop while (*pb_count < idx + 1):
 *        ├── a->value <= arr[idx]?
 *        │         ├── [Yes] ──> pb(config), (*pb_count)++
 *        │         └── [No]  ──> ra(config), rotated++
 *            │
 *            ▼
 *   !whole? ───[Yes]───> while (rotated-- > 0) rra(config)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *  - arr {int *}: Sorted subarray representing current slice values.
 *  - size {int}: Element count of the current recursive sub-slice.
 *  - pb_count {int *}: Output counter tracking elements pushed to B.
 *
 * RETURNS:
 *  - void: Modifies stacks A and B, updating *pb_count in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(size) operations.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - When whole is true (entire stack A), omits rra restoration to save
 *    operations since the entire circular ring was processed.
 * =========================================================================
 */
void	median_partation_a(t_config *config, int *arr, int size, int *pb_count)
{
	int	whole;
	int	rotated;
	int	idx;

	idx = calculate_index(size, config->pivot, 1);
	whole = (config->nums.size_a == size);
	rotated = 0;
	while (*pb_count < idx + 1)
	{
		if (config->nums.a->value <= arr[idx])
		{
			pb(config);
			(*pb_count)++;
		}
		else
		{
			ra(config);
			rotated++;
		}
	}
	while (!whole && rotated-- > 0)
		rra(config);
}

/**
 * =========================================================================
 * FUNCTION: median_partation_b
 * =========================================================================
 * OVERVIEW:
 *  Partitions a sub-slice of stack B into stack A using the pivot value
 *  at arr[idx]. Elements >= arr[idx] are pushed to A via pa. Elements
 *  < arr[idx] are rotated down via rb. If partitioning a partial stack,
 *  unwinds rotations via rrb to maintain remaining stack ordering.
 *
 * EXECUTION SCHEMA:
 *
 *   idx = calculate_index(size, pivot, 0)
 *   whole = (size_b == size)
 *            │
 *            ▼
 *   Loop while (*pa_count < size - idx):
 *        ├── b->value >= arr[idx]?
 *        │         ├── [Yes] ──> pa(config), (*pa_count)++
 *        │         └── [No]  ──> rb(config), rotated++
 *            │
 *            ▼
 *   !whole? ───[Yes]───> while (rotated-- > 0) rrb(config)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *  - arr {int *}: Sorted subarray representing current slice values.
 *  - size {int}: Element count of the current recursive sub-slice.
 *  - pa_count {int *}: Output counter tracking elements pushed to A.
 *
 * RETURNS:
 *  - void: Modifies stacks A and B, updating *pa_count in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(size) operations.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - When whole is true (entire stack B), omits rrb restoration to save
 *    operations.
 *  - Operates symmetrically to median_partation_a to support quicksort.
 * =========================================================================
 */
void	median_partation_b(t_config *config, int *arr, int size, int *pa_count)
{
	int	whole;
	int	rotated;
	int	idx;

	idx = calculate_index(size, config->pivot, 0);
	whole = (config->nums.size_b == size);
	rotated = 0;
	while (*pa_count < size - idx)
	{
		if (config->nums.b->value >= arr[idx])
		{
			pa(config);
			(*pa_count)++;
		}
		else
		{
			rb(config);
			rotated++;
		}
	}
	while (!whole && rotated-- > 0)
		rrb(config);
}
