/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 10:16:11 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/08 09:24:22 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: ra
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'ra' (rotate A) instruction. Shifts all elements in stack A
 *  upwards by one position, making the former top element become the tail.
 *  In a circular doubly-linked list, this is achieved in O(1) by advancing
 *  the head pointer to the next node.
 *
 * EXECUTION SCHEMA:
 *
 *   Before:  [Node A (head)] <───> [Node B] <───> [Node C (tail)] ↺
 *                   │
 *                   ▼ (head = head->next)
 *   After:   [Node B (head)] <───> [Node C] <───> [Node A (tail)] ↺
 *                   │
 *                   ▼
 *            ft_printf("ra\n", 1)
 *            config->metrics.ra++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Updates stack A head pointer in place, outputs to stdout, and
 *    increments the 'ra' metric.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe no-op: Returns immediately without printing or incrementing
 *    metrics if stack A is empty or contains only a single element.
 * =========================================================================
 */
void	ra(t_config *config)
{
	if (!config->nums.a || config->nums.a == config->nums.a->next)
		return ;
	config->nums.a = config->nums.a->next;
	if (!config->silent)
		ft_printf("ra\n", 1);
	config->metrics.ra++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: rb
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'rb' (rotate B) instruction. Shifts all elements in stack B
 *  upwards by one position, making the former top element become the tail.
 *  In a circular doubly-linked list, this is achieved in O(1) by advancing
 *  the head pointer to the next node.
 *
 * EXECUTION SCHEMA:
 *
 *   Before:  [Node A (head)] <───> [Node B] <───> [Node C (tail)] ↺
 *                   │
 *                   ▼ (head = head->next)
 *   After:   [Node B (head)] <───> [Node C] <───> [Node A (tail)] ↺
 *                   │
 *                   ▼
 *            ft_printf("rb\n", 1)
 *            config->metrics.rb++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Updates stack B head pointer in place, outputs to stdout, and
 *    increments the 'rb' metric.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe no-op: Returns immediately without printing or incrementing
 *    metrics if stack B is empty or contains only a single element.
 * =========================================================================
 */
void	rb(t_config *config)
{
	if (!config->nums.b || config->nums.b == config->nums.b->next)
		return ;
	config->nums.b = config->nums.b->next;
	if (!config->silent)
		ft_printf("rb\n", 1);
	config->metrics.rb++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: rr
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'rr' instruction (simultaneous rotate of A and B). Rotates
 *  each stack upwards by one element if it contains at least two nodes.
 *  Uses a flag to track state changes, suppressing terminal output and
 *  metric increments if neither stack was modified.
 *
 * EXECUTION SCHEMA:
 *
 *   moved = false
 *         │
 *         ├── Stack A (size >= 2)? ──> a = a->next, moved = true
 *         │
 *         └── Stack B (size >= 2)? ──> b = b->next, moved = true
 *                     │
 *                     ▼
 *                !moved? ───[Yes]───> Return (Silent no-op)
 *                     │ [No]
 *                     ▼
 *            ft_printf("rr\n", 1)
 *            config->metrics.rr++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Updates qualifying stacks, writes output, and updates metrics.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Silent No-Op: If neither stack has >= 2 elements, returns immediately
 *    without emitting "rr\n" or updating counters.
 *  - Partial Action: Safely rotates whichever stack qualifies if only one
 *    has at least 2 nodes.
 *  - Memory Safety: Prevents invalid pointer dereferences on NULL or
 *    single-node circular loops.
 * =========================================================================
 */
void	rr(t_config *config)
{
	bool	moved;

	moved = false;
	if (config->nums.a && config->nums.a != config->nums.a->next)
	{
		config->nums.a = config->nums.a->next;
		moved = true;
	}
	if (config->nums.b && config->nums.b != config->nums.b->next)
	{
		config->nums.b = config->nums.b->next;
		moved = true;
	}
	if (!moved)
		return ;
	if (!config->silent)
		ft_printf("rr\n", 1);
	config->metrics.rr++;
	config->metrics.total++;
}
