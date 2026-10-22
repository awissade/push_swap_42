/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 13:50:31 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 14:13:58 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: is_sorted_a
 * =========================================================================
 * OVERVIEW:
 *  Verifies whether stack A is currently arranged in strictly ascending
 *  order from the head node down to the tail. Traverses the circular list
 *  until wrapping back to head, detecting any inversions.
 *
 * EXECUTION SCHEMA:
 *
 *   !head? ──────────────────────────[Yes]──> Return 1 (Trivially sorted)
 *     │ [No]
 *     ▼
 *   tmp = head
 *   Loop while (tmp->next != head):
 *     │
 *     ├── tmp->value > tmp->next->value?
 *     │         ├── [Yes] ──────────────────> Return 0 (Unsorted)
 *     │         └── [No]  ──────────────────> tmp = tmp->next
 *     │
 *     ▼ (Traversed entire ring)
 *   Return 1 (Sorted)
 *
 * PARAMETERS:
 *  - head {t_stack *}: Pointer to the top element of stack A.
 *
 *  - {int}: 1 if stack A is completely sorted or empty; 0 if unsorted.
 *
 * RETURNS:
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the number of elements in stack A.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Returns 1 for NULL pointer or single-node circular stacks.
 *  - Assumes strictly unique elements; treats equal values as not inverted.
 * =========================================================================
 */
int	is_sorted_a(t_stack *head)
{
	t_stack	*tmp;

	if (!head)
		return (1);
	tmp = head;
	while (tmp->next != head)
	{
		if (tmp->value > tmp->next->value)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

/**
 * =========================================================================
 * FUNCTION: fit_between
 * =========================================================================
 * OVERVIEW:
 *  Determines whether a candidate value logically belongs between the
 *  preceding node (stack->prev) and the current node (stack). Handles both
 *  standard ascending interior intervals and wrap-around extrema boundaries.
 *
 * EXECUTION SCHEMA:
 *
 *   before = stack->prev->value
 *   after  = stack->value
 *        │
 *        ├── before < after (Standard ascending window)
 *        │     └── before < value && value < after? ──> 1 (Fits)
 *        │
 *        └── before >= after (Wrap-around / Min-Max boundary)
 *              └── value > before || value <= after? ──> 1 (Fits)
 *
 * PARAMETERS:
 *  - stack {t_stack *}: Target node candidate in circular stack A.
 *  - value {int}: Value from stack B seeking insertion.
 *
 * RETURNS:
 *  - {int}: 1 if 'value' fits immediately before 'stack'; 0 otherwise.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Boundary wrap condition handles values smaller than the current minimum
 *    or larger than the current maximum in stack A.
 * =========================================================================
 */
static int	fit_between(t_stack *stack, int value)
{
	int	before;
	int	after;

	before = stack->prev->value;
	after = stack->value;
	if (before < after)
		return (before < value && after > value);
	return (value > before || value <= after);
}

/**
 * =========================================================================
 * FUNCTION: step_to_target
 * =========================================================================
 * OVERVIEW:
 *  Calculates the forward offset (number of ra steps from head) needed to
 *  position the correct insertion slot of stack A at the top.
 *
 * EXECUTION SCHEMA:
 *
 *   tmp = a, step = 0
 *        │
 *        ↺ Loop while (!fit_between(tmp, value)):
 *             │
 *             ├── tmp = tmp->next
 *             └── step++
 *        │
 *        ▼
 *   Return step
 *
 * PARAMETERS:
 *  - a {t_stack *}: Current head pointer of stack A.
 *  - value {int}: Numerical value to be inserted.
 *
 * RETURNS:
 *  - {int}: Zero-based forward step distance to the matching insertion node.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is current node count in stack A.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Guaranteed to terminate because a valid insertion boundary exists in
 *    any sorted circular list.
 * =========================================================================
 */
static int	step_to_target(t_stack *a, int value)
{
	t_stack	*tmp;
	int		step;

	tmp = a;
	step = 0;
	while (!fit_between(tmp, value))
	{
		tmp = tmp->next;
		step++;
	}
	return (step);
}

/**
 * =========================================================================
 * FUNCTION: insert_cheapest
 * =========================================================================
 * OVERVIEW:
 *  Evaluates the total mechanical cost of inserting each node from stack B
 *  into its target slot in stack A. Selects the element requiring the
 *  minimum overall operations, executes joint/individual rotations, and
 *  pushes the element to stack A via pa.
 *
 * EXECUTION SCHEMA:
 *
 *   best.cost = INT_MAX
 *        │
 *        ↺ For each node in B (index i from 0 to size_b - 1):
 *             │
 *             ├── af = step_to_target(nums.a, tmp->value)
 *             ├── best_way(config, af, i, &move)
 *             └── move.cost < best.cost? ──> best = move
 *             │
 *             ▼ (Next node in B)
 *   rotate_pair(config, best.x, best.y) ──> Align both stacks
 *   pa(config)                          ──> Push element onto A
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and dimensions.
 *
 * RETURNS:
 *  - void: Mutates both stacks and emits rotation and push operations.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(size_a * size_b) total search cost per insertion.
 *  - Space Complexity: O(1) auxiliary stack space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Requires config->nums.size_b > 0.
 *  - Relies on best_way to resolve optimal directions (rr, rrr, or mixed).
 * =========================================================================
 */
static void	insert_cheapest(t_config *config)
{
	t_stack	*tmp;
	t_move	move;
	t_move	best;
	int		i;

	best.cost = INT_MAX;
	tmp = config->nums.b;
	i = 0;
	while (i < config->nums.size_b)
	{
		best_way(config, step_to_target(config->nums.a, tmp->value), i, &move);
		if (move.cost < best.cost)
			best = move;
		tmp = tmp->next;
		i++;
	}
	rotate_pair(config, best.x, best.y);
	pa(config);
}

/**
 * =========================================================================
 * FUNCTION: simple
 * =========================================================================
 * OVERVIEW:
 *  Driver routine for the simple sorting strategy (Turk algorithm with LIS).
 *  Preserves the Longest Increasing Subsequence in stack A, pushes remaining
 *  nodes to B, greedily reinserts each element from B at minimum cost, and
 *  rotates the minimum element to the top.
 *
 * EXECUTION SCHEMA:
 *
 *   is_sorted_a(nums.a)? ───────[Yes]───> Return (Already sorted)
 *        │ [No]
 *        ▼
 *   push_others(config) ────────────────> Keep LIS in A; push rest to B
 *        │
 *        ▼
 *   Loop while (nums.size_b > 0):
 *        └── insert_cheapest(config) ───> Greedily insert lowest-cost node
 *        │
 *        ▼
 *   rotate_a(step_to_target(INT_MIN)) ──> Rotate absolute minimum to head
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and telemetry.
 *
 * RETURNS:
 *  - void: Modifies stack A until fully sorted in ascending order.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N^2) worst-case insertion and search steps.
 *  - Space Complexity: O(N) heap memory used during initial LIS analysis.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Early exit if stack A is already sorted.
 *  - Uses INT_MIN as a universal probe to locate the absolute minimum node.
 * =========================================================================
 */
void	simple(t_config *config)
{
	config->chosen = SIMPLE;
	config->nums.size_a = config->nums.size;
	config->nums.size_b = 0;
	if (is_sorted_a(config->nums.a))
		return ;
	push_others(config);
	while (config->nums.size_b > 0)
		insert_cheapest(config);
	rotate_a(config, step_to_target(config->nums.a, INT_MIN));
}
