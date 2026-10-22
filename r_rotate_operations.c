/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_rotate_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 10:57:22 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/25 11:21:54 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: rra
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'rra' (reverse rotate A) instruction. Shifts all elements
 *  downward by one position, making the tail become the new head. In a
 *  circular doubly-linked list, this is achieved in O(1) by shifting the
 *  head pointer backwards to the previous node.
 *
 * EXECUTION SCHEMA:
 *
 *   Before:  [Node A (head)] <───> [Node B] <───> [Node C (tail)] ↺
 *                   │
 *                   ▼ (head = head->prev)
 *   After:   [Node C (head)] <───> [Node A] <───> [Node B (tail)] ↺
 *                   │
 *                   ▼
 *            ft_printf("rra\n", 1)
 *            config->metrics.rra++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Updates stack A head pointer in place, outputs to stdout, and
 *    increments the 'rra' metric.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe no-op: Returns immediately without printing or incrementing
 *    counters if stack A is empty or contains only one node.
 * =========================================================================
 */
void	rra(t_config *config)
{
	if (!config->nums.a || config->nums.a == config->nums.a->next)
		return ;
	config->nums.a = config->nums.a->prev;
	if (!config->silent)
		ft_printf("rra\n", 1);
	config->metrics.rra++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: rrb
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'rrb' (reverse rotate B) instruction. Shifts all elements
 *  downward by one position, making the tail become the new head. In a
 *  circular doubly-linked list, this is achieved in O(1) by shifting the
 *  head pointer backwards to the previous node.
 *
 * EXECUTION SCHEMA:
 *
 *   Before:  [Node A (head)] <───> [Node B] <───> [Node C (tail)] ↺
 *                   │
 *                   ▼ (head = head->prev)
 *   After:   [Node C (head)] <───> [Node A] <───> [Node B (tail)] ↺
 *                   │
 *                   ▼
 *            ft_printf("rrb\n", 1)
 *            config->metrics.rrb++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Updates stack B head pointer in place, outputs to stdout, and
 *    increments the 'rrb' metric.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe no-op: Returns immediately without printing or incrementing
 *    counters if stack B is empty or contains only one node.
 * =========================================================================
 */
void	rrb(t_config *config)
{
	if (!config->nums.b || config->nums.b == config->nums.b->next)
		return ;
	config->nums.b = config->nums.b->prev;
	if (!config->silent)
		ft_printf("rrb\n", 1);
	config->metrics.rrb++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: rrr
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'rrr' instruction (simultaneous reverse rotate of A and B).
 *  Shifts each qualifying stack downward by one element if it contains at
 *  least two nodes. Suppresses stdout printing and metric tracking if neither
 *  stack was shifted.
 *
 * EXECUTION SCHEMA:
 *
 *   moved = false
 *         │
 *         ├── Stack A (size >= 2)? ──> a = a->prev, moved = true
 *         │
 *         └── Stack B (size >= 2)? ──> b = b->prev, moved = true
 *                     │
 *                     ▼
 *                !moved? ───[Yes]───> Return (Silent no-op)
 *                     │ [No]
 *                     ▼
 *            ft_printf("rrr\n", 1)
 *            config->metrics.rrr++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Updates eligible stack heads in place, writes to stdout, and
 *    increments the 'rrr' metric.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Silent No-Op: Returns without action or printing if both stacks have
 *    fewer than 2 elements.
 *  - Partial Action: Safely rotates whichever stack qualifies if only one
 *    stack has 2 or more nodes.
 *  - Pointer Safety: Guards prevent null dereferences on uninitialized or
 *    single-node circular stacks.
 * =========================================================================
 */
void	rrr(t_config *config)
{
	bool	moved;

	moved = false;
	if (config->nums.a && config->nums.a != config->nums.a->next)
	{
		config->nums.a = config->nums.a->prev;
		moved = true;
	}
	if (config->nums.b && config->nums.b != config->nums.b->next)
	{
		config->nums.b = config->nums.b->prev;
		moved = true;
	}
	if (!moved)
		return ;
	if (!config->silent)
		ft_printf("rrr\n", 1);
	config->metrics.rrr++;
	config->metrics.total++;
}
