/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 03:14:33 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/29 03:16:33 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: ft_isqrt
 * =========================================================================
 * OVERVIEW:
 *  Computes the integer square root of a non-negative integer using
 *  Newton's method (Babylonian method) with integer arithmetic, returning
 *  floor(sqrt(size)).
 *
 * EXECUTION SCHEMA:
 *
 *   size <= 0? ──[Yes]──> Return 0
 *   size == 1? ──[Yes]──> Return 1
 *        │ [No]
 *        ▼
 *   guess = size / 2
 *        │
 *        ↺ Loop:
 *        next_guess = (guess + (size / guess)) / 2
 *             │
 *             ├── next_guess < guess ──> guess = next_guess (Refine)
 *             │
 *             └── next_guess >= guess ─> break (Converged)
 *        │
 *        ▼
 *   Return guess
 *
 * PARAMETERS:
 *  - size {int}: Non-negative integer value to compute root for.
 *
 * RETURNS:
 *  - {int}: Floor of the square root of 'size'; 0 if size <= 0.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(log(log(size))) quadratic convergence.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Non-positive inputs (size <= 0) return 0 safely.
 *  - Integer truncation naturally prevents over-estimation.
 * =========================================================================
 */
static int	ft_isqrt(int size)
{
	int	guess;
	int	next_guess;

	if (size <= 0)
		return (0);
	if (size == 1)
		return (1);
	guess = size / 2;
	while (1)
	{
		next_guess = (guess + (size / guess)) / 2;
		if (next_guess < guess)
			guess = next_guess;
		else
			break ;
	}
	return (guess);
}

/**
 * =========================================================================
 * FUNCTION: push_all_to_b
 * =========================================================================
 * OVERVIEW:
 *  Partitions and pushes all elements from stack A into stack B using a
 *  sliding window (chunk sort). Compares values against sorted indices:
 *  values below the window baseline are sent to the bottom of B (pb + rb),
 *  values within the current window stay on top of B (pb), and larger values
 *  are rotated to the back of A (ra).
 *
 * EXECUTION SCHEMA:
 *
 *   While stack A is not empty:
 *     end = min(start + range, size - 1)
 *            │
 *            ├── A->value <= arr[start]? ──> pb + rb, start++
 *            │   (Smaller chunk: pushed to bottom of B)
 *            │
 *            ├── A->value <= arr[end]?   ──> pb, start++
 *            │   (Current chunk: pushed to top of B)
 *            │
 *            └── Otherwise               ──> ra
 *                (Out of window: rotate to bottom of A)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and sorted array.
 *  - range {int}: Size of the dynamic sliding index window.
 *
 * RETURNS:
 *  - void: Empties stack A and populates stack B in two-tiered chunks.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N * range) total operations across passes.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Clamps 'end' to 'nums.size - 1' to avoid out-of-bounds array reads.
 *  - Assumes 'nums.arr' contains the pre-sorted sequence of elements.
 * =========================================================================
 */
static void	push_all_to_b(t_config *config, int range)
{
	int	start;
	int	end;

	start = 0;
	while (config->nums.a)
	{
		end = start + range;
		if (end >= config->nums.size)
			end = config->nums.size - 1;
		if (config->nums.a->value <= config->nums.arr[start])
		{
			pb(config);
			rb(config);
			start++;
		}
		else if (config->nums.a->value <= config->nums.arr[end])
		{
			pb(config);
			start++;
		}
		else
			ra(config);
	}
}

/**
 * =========================================================================
 * FUNCTION: get_pos
 * =========================================================================
 * OVERVIEW:
 *  Traverses stack B sequentially from top to tail to locate the zero-based
 *  offset index of an element matching 'target'.
 *
 * EXECUTION SCHEMA:
 *
 *   b ──> [Node 0] ──> [Node 1] ──> ... ──> [Target]
 *           │            │                    │
 *        pos = 0      pos = 1              Return pos
 *
 * PARAMETERS:
 *  - b {t_stack *}: Pointer to the head of stack B.
 *  - target {int}: Target value whose index is being queried.
 *
 * RETURNS:
 *  - {int}: Zero-based distance from head of stack B to target node.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(K), where K is distance to target (at most size B).
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Assumes 'target' exists in stack B to prevent unbounded circular loops.
 * =========================================================================
 */
static int	get_pos(t_stack *b, int target)
{
	int	pos;

	pos = 0;
	while (b->value != target)
	{
		pos++;
		b = b->next;
	}
	return (pos);
}

/**
 * =========================================================================
 * FUNCTION: push_back_to_a
 * =========================================================================
 * OVERVIEW:
 *  Restores elements from stack B back to stack A in strictly descending
 *  order of rank (from largest to smallest). Chooses the cheapest rotation
 *  path (rb vs rrb) based on target distance relative to midpoint, brings
 *  each target to the top of B, and pushes it onto A (pa).
 *
 * EXECUTION SCHEMA:
 *
 *   Target rank = size - 1 down to 0:
 *          │
 *          ▼
 *   pos = get_pos(B, arr[target])
 *          │
 *          ├── pos <= size_b / 2 ──> Rotate upwards: repeat 'pos' times rb
 *          │
 *          └── pos >  size_b / 2 ──> Rotate downwards: repeat (size - pos) rrb
 *          │
 *          ▼
 *   pa(config) ──> Top of B pushed to A
 *   target--, size_b--
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and sorted array.
 *  - range {int}: Unused chunk parameter retained for signature consistency.
 *
 * RETURNS:
 *  - void: Empties stack B and leaves stack A fully sorted.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N^2) worst-case search and rotation steps.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Decrements 'size_b' concurrently with 'target' to keep midpoint
 *    calculations accurate throughout drainage.
 * =========================================================================
 */
static void	push_back_to_a(t_config *config)
{
	int	target;
	int	pos;
	int	size_b;

	target = config->nums.size - 1;
	size_b = config->nums.size;
	while (target >= 0)
	{
		pos = get_pos(config->nums.b, config->nums.arr[target]);
		if (pos <= size_b / 2)
			while (pos-- > 0)
				rb(config);
		else
			while (pos++ < size_b)
				rrb(config);
		pa(config);
		target--;
		size_b--;
	}
}

/**
 * =========================================================================
 * FUNCTION: medium
 * =========================================================================
 * OVERVIEW:
 *  High-level coordinator for the medium sorting routine (chunk/sliding
 *  window sort). Calculates dynamic window width using square root heuristic,
 *  partitions stack A into pre-grouped layers in B, and reconstructs a
 *  sorted stack A.
 *
 * EXECUTION SCHEMA:
 *
 *   config->nums.a (Unsorted)
 *          │
 *          ▼
 *   range = ft_isqrt(size) * 1.5
 *          │
 *          ▼
 *   push_all_to_b(config, range)   ───> Stacks layered chunks into B
 *          │
 *          ▼
 *   push_back_to_a(config, range)  ───> Pulls sorted elements back to A
 *          │
 *          ▼
 *   config->nums.a (Fully Sorted)
 *
 * PARAMETERS:
 *  - config {t_config *}: State struct containing stacks, metrics, and array.
 *
 * RETURNS:
 *  - void: Modifies stacks in place until stack A is ordered.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N * sqrt(N)) average operations.
 *  - Space Complexity: O(1) auxiliary memory beyond existing buffers.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Relies on 'config->nums.arr' having been sorted during initialization.
 *  - Scaling factor 1.5 optimizes instruction counts for size ~100 to 500.
 * =========================================================================
 */
void	medium(t_config *config)
{
	int	range;

	config->chosen = MEDIUM;
	if (is_sorted_a(config->nums.a))
		return ;
	range = ft_isqrt(config->nums.size) * 1.5;
	push_all_to_b(config, range);
	push_back_to_a(config);
}
