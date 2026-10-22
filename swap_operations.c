/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 11:11:53 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/25 09:54:39 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: swap_top_two
 * =========================================================================
 * OVERVIEW:
 *  Swaps the first two elements at the top of a circular doubly-linked
 *  stack. If the stack has fewer than two elements, the function does
 *  nothing. For a two-element stack, it updates the head pointer. For three
 *  or more elements, it splices and swaps the links between the tail, the
 *  first two nodes, and the third node.
 *
 * EXECUTION SCHEMA:
 *
 *   Case 1: Fewer than 2 nodes (!*head || *head == (*head)->next)
 *     Return (No-op)
 *
 *   Case 2: Exactly 2 nodes (second->next == first)
 *     [*head = first] <───> [second] ↺
 *           │
 *           ▼
 *     [*head = second] <───> [first] ↺
 *
 *   Case 3: 3+ nodes
 *     Before: [Tail] <──> [First (head)] <──> [Second] <──> [Third] ...
 *                                │                │
 *                                └────── Swap ────┘
 *                                        │
 *     After:  [Tail] <──> [Second (head)] <──> [First] <──> [Third] ...
 *
 * PARAMETERS:
 *  - head {t_stack **}: Address of the stack head pointer to modify.
 *
 * RETURNS:
 *  - void: Modifies node pointers and stack head in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safely handles empty stacks (*head == NULL) and single-node stacks.
 *  - Preserves circular doubly-linked consistency across all neighbors.
 * =========================================================================
 */
static void	swap_top_two(t_stack **head)
{
	t_stack	*first;
	t_stack	*second;

	if (!*head || (*head) == (*head)->next)
		return ;
	first = (*head);
	second = first->next;
	if (second->next == first)
	{
		(*head) = second;
		return ;
	}
	second->next->prev = first;
	first->prev->next = second;
	first->next = second->next;
	second->prev = first->prev;
	second->next = first;
	first->prev = second;
	(*head) = second;
}

/**
 * =========================================================================
 * FUNCTION: sa
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'sa' (swap A) instruction. Swaps the first two elements at
 *  the top of stack A, prints "sa\n" to standard output, and increments
 *  the operation counter.
 *
 * EXECUTION SCHEMA:
 *
 *   config->nums.a
 *         │
 *         ▼
 *   swap_top_two(&config->nums.a)
 *         │
 *         ▼
 *   ft_printf("sa\n", 1)
 *         │
 *         ▼
 *   config->metrics.sa++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Mutates stack A, writes output, and updates metrics.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - If stack A has fewer than 2 elements, the swap is a silent no-op,
 *    but "sa\n" is still emitted and metrics are incremented.
 * =========================================================================
 */
void	sa(t_config *config)
{
	swap_top_two(&config->nums.a);
	if (!config->silent)
		ft_printf("sa\n", 1);
	config->metrics.sa++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: sb
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'sb' (swap B) instruction. Swaps the first two elements at
 *  the top of stack B, prints "sb\n" to standard output, and increments
 *  the operation counter.
 *
 * EXECUTION SCHEMA:
 *
 *   config->nums.b
 *         │
 *         ▼
 *   swap_top_two(&config->nums.b)
 *         │
 *         ▼
 *   ft_printf("sb\n", 1)
 *         │
 *         ▼
 *   config->metrics.sb++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Mutates stack B, writes output, and updates metrics.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - If stack B has fewer than 2 elements, the swap is a silent no-op,
 *    but "sb\n" is still emitted and metrics are incremented.
 * =========================================================================
 */
void	sb(t_config *config)
{
	swap_top_two(&config->nums.b);
	if (!config->silent)
		ft_printf("sb\n", 1);
	config->metrics.sb++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: ss
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'ss' instruction (simultaneous swap of A and B). Swaps the
 *  first two elements of both stack A and stack B, prints "ss\n" to
 *  standard output, and increments the 'ss' metric counter.
 *
 * EXECUTION SCHEMA:
 *
 *   config->nums.a             config->nums.b
 *         │                          │
 *         ▼                          ▼
 *   swap_top_two(a)            swap_top_two(b)
 *         └────────────┬─────────────┘
 *                      │
 *                      ▼
 *              ft_printf("ss\n", 1)
 *                      │
 *                      ▼
 *              config->metrics.ss++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Mutates stacks A and B, writes output, and updates metrics.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Operates safely even if either stack has fewer than two elements.
 *  - Counts as a single joint operation in metrics rather than two.
 * =========================================================================
 */
void	ss(t_config *config)
{
	swap_top_two(&config->nums.a);
	swap_top_two(&config->nums.b);
	if (!config->silent)
		ft_printf("ss\n", 1);
	config->metrics.ss++;
	config->metrics.total++;
}
