/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 13:16:52 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 13:50:09 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: sort_b
 * =========================================================================
 * OVERVIEW:
 *  Recursive quicksort subroutine for stack B segments. Dispatches small
 *  slices (<= 30) directly to select_b. For larger segments, partitions
 *  values relative to a reference pivot by pushing elements >= pivot to
 *  stack A, then recursively sorts both halves.
 *
 * EXECUTION SCHEMA:
 *
 *   size <= 0?  ──[Yes]──> Return (No-op)
 *   size <= 30? ──[Yes]──> select_b(config, arr, size)
 *        │ [No]
 *        ▼
 *   median_partation_b(&pa_count)
 *        ├── Pushes elements >= pivot to A (count: pa_count)
 *        └── Retains elements < pivot in B (count: size - pa_count)
 *        │
 *        ├── sort_a(arr + (size - pa_count), pa_count) ──> Sort upper in A
 *        └── sort_b(arr, size - pa_count)             ──> Sort lower in B
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and settings.
 *  - arr {int *}: Reference sorted subarray matching elements in slice.
 *  - size {int}: Number of elements in the current stack B partition.
 *
 * RETURNS:
 *  - void: Recursively sorts and moves elements back to stack A.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N log N) average quicksort partitioning.
 *  - Space Complexity: O(log N) call stack frames due to recursion.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Non-positive sizes (size <= 0) return immediately.
 *  - Base threshold (<= 30) bypasses recursive overhead via selection.
 * =========================================================================
 */
void	sort_b(t_config *config, int *arr, int size)
{
	int	pa_count;

	pa_count = 0;
	if (size <= 0)
		return ;
	if (size <= 30)
		return (select_b(config, arr, size));
	median_partation_b(config, arr, size, &pa_count);
	sort_a(config, arr + (size - pa_count), pa_count);
	sort_b(config, arr, size - pa_count);
}

/**
 * =========================================================================
 * FUNCTION: sort_a
 * =========================================================================
 * OVERVIEW:
 *  Recursive quicksort coordinator for stack A segments. Handles tiny
 *  slices (sizes 2 and 3) using specialized base-case routines. For larger
 *  segments, partitions smaller values into stack B, recurses on the
 *  remaining upper elements in A, and recurses on B.
 *
 * EXECUTION SCHEMA:
 *
 *   size < 2?  ──[Yes]──> Return (Already sorted)
 *   size == 2? ──[Yes]──> sort_two(config)
 *   size == 3? ──[Yes]──> sort_three(config)
 *        │ [No]
 *        ▼
 *   median_partation_a(&pb_count)
 *        ├── Pushes elements <= pivot to B (count: pb_count)
 *        └── Retains elements > pivot in A (count: size - pb_count)
 *        │
 *        ├── sort_a(arr + pb_count, size - pb_count) ──> Sort upper in A
 *        └── sort_b(arr, pb_count)                   ──> Sort lower in B
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and settings.
 *  - arr {int *}: Reference sorted subarray matching elements in slice.
 *  - size {int}: Number of elements in the current stack A partition.
 *
 * RETURNS:
 *  - void: Modifies stacks in place until slice is sorted in ascending order.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N log N) average divide-and-conquer partitioning.
 *  - Space Complexity: O(log N) recursive call stack frames.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Segments smaller than 2 require no operations and return cleanly.
 *  - Leaves larger elements in A so returning elements from B rest on top.
 * =========================================================================
 */
void	sort_a(t_config *config, int *arr, int size)
{
	int	pb_count;

	pb_count = 0;
	if (size < 2)
		return ;
	if (size == 2)
		return (sort_two(config));
	if (size == 3)
		return (sort_three(config));
	median_partation_a(config, arr, size, &pb_count);
	sort_a(config, arr + pb_count, size - pb_count);
	sort_b(config, arr, pb_count);
}

/**
 * =========================================================================
 * FUNCTION: complex
 * =========================================================================
 * OVERVIEW:
 *  Top-level entry point for the complex sorting strategy (Quicksort).
 *  Sets execution telemetry flag, verifies whether stack A is already
 *  ordered, and launches recursive divide-and-conquer partitioning.
 *
 * EXECUTION SCHEMA:
 *
 *   config->chosen = COMPLEX
 *        │
 *        ▼
 *   is_sorted_a(nums.a)? ──[Yes]──> Return (Early exit)
 *        │ [No]
 *        ▼
 *   sort_a(config, nums.arr, nums.size) ──> Launch quicksort recursion
 *
 * PARAMETERS:
 *  - config {t_config *}: Program state containing stacks, sorted array,
 *                         and execution telemetry metrics.
 *
 * RETURNS:
 *  - void: Sorts entire stack A in place in ascending order.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N log N) average operations across entire stack.
 *  - Space Complexity: O(log N) recursive call stack depth.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Exits immediately with 0 instructions if stack A is already sorted.
 *  - Assumes config->nums.arr contains the pre-sorted sequence of elements.
 * =========================================================================
 */
void	complex(t_config *config)
{
	config->chosen = COMPLEX;
	if (is_sorted_a(config->nums.a))
		return ;
	sort_a(config, config->nums.arr, config->nums.size);
}
