/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:33:40 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 14:33:44 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: detach_top
 * =========================================================================
 * OVERVIEW:
 *  Unlinks and extracts the top node from a circular doubly-linked stack.
 *  If the stack contains only one node, sets the stack pointer to NULL.
 *  Otherwise, advances head to the next node and restores circular links
 *  between the new head and the tail.
 *
 * EXECUTION SCHEMA:
 *
 *   Case 1: Single Node (node == node->next)
 *     [*head] ──> [Node A] ↺
 *                    │
 *                    ▼
 *     [*head] = NULL, return [Node A]
 *
 *   Case 2: Multiple Nodes
 *     [Tail] <───> [Node A (head)] <───> [Node B] <───> ...
 *                        │
 *                        ▼
 *     *head = Node B
 *     Node B->prev = Tail
 *     Tail->next   = Node B
 *                        │
 *                        ▼
 *     [Tail] <─────────────────────────> [Node B (new head)]
 *     Return Node A
 *
 * PARAMETERS:
 *  - head {t_stack **}: Address of the stack head pointer to detach from.
 *
 * RETURNS:
 *  - {t_stack *}: Pointer to the detached top node.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Assumes *head is non-NULL when invoked.
 *  - Correctly collapses a 1-node circular list to NULL.
 *  - Detached node retains its previous internal pointers until reassigned.
 * =========================================================================
 */
static t_stack	*detach_top(t_stack **head)
{
	t_stack	*node;

	node = (*head);
	if (node == (*head)->next)
		*head = NULL;
	else
	{
		(*head) = node->next;
		(*head)->prev = node->prev;
		node->prev->next = (*head);
	}
	return (node);
}

/**
 * =========================================================================
 * FUNCTION: attach_top
 * =========================================================================
 * OVERVIEW:
 *  Attaches an isolated node to the top of a circular doubly-linked stack.
 *  Initializes self-referencing circular pointers if the target stack is
 *  empty, or splices the node between the tail and current head.
 *
 * EXECUTION SCHEMA:
 *
 *   Case 1: Empty Stack (*head == NULL)
 *     node ──> [ node ] ↺ (next and prev point to node itself)
 *     *head = node
 *
 *   Case 2: Populated Stack
 *     Insert [node] before [*head] and after [Tail]:
 *
 *     [Tail] <───                       ───> [*head (old)]
 *        ▲                                         ▲
 *        │           ┌─────────────────┐           │
 *        └── prev ───│   node (new)    │── next ───┘
 *                    └─────────────────┘
 *     *head = node
 *
 * PARAMETERS:
 *  - head {t_stack **}: Address of the destination stack head pointer.
 *  - node {t_stack *}: Isolated node to push onto the stack.
 *
 * RETURNS:
 *  - void: Updates the list and head reference in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safely initializes empty stacks into closed circular loops.
 *  - Maintains circular bidirectional integrity without traversing the list.
 * =========================================================================
 */
static void	attach_top(t_stack **head, t_stack *node)
{
	if (!(*head))
	{
		(*head) = node;
		node->next = node;
		node->prev = node;
	}
	else
	{
		node->next = (*head);
		node->prev = (*head)->prev;
		(*head)->prev->next = node;
		(*head)->prev = node;
		(*head) = node;
	}
}

/**
 * =========================================================================
 * FUNCTION: pa
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'pa' (push A) instruction. Pops the top node from stack B,
 *  pushes it onto the top of stack A, logs "pa\n" to standard output, and
 *  increments the operation counter.
 *
 * EXECUTION SCHEMA:
 *
 *   config->nums.b == NULL? ───[Yes]───> Return (No-op)
 *             │ [No]
 *             ▼
 *     node = detach_top(&config->nums.b)
 *             │
 *             ▼
 *     attach_top(&config->nums.a, node)
 *             │
 *             ▼
 *     ft_printf("pa\n", 1)
 *     config->metrics.pa++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Modifies stacks A and B, writes output, updates metrics.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe no-op: Returns immediately if stack B is empty without output.
 *  - Preserves node payload without reallocation or memory duplication.
 * =========================================================================
 */
void	pa(t_config *config)
{
	t_stack	*node;

	if (!config->nums.b)
		return ;
	node = detach_top(&config->nums.b);
	attach_top(&config->nums.a, node);
	config->nums.size_b--;
	config->nums.size_a++;
	if (!config->silent)
		ft_printf("pa\n", 1);
	config->metrics.pa++;
	config->metrics.total++;
}

/**
 * =========================================================================
 * FUNCTION: pb
 * =========================================================================
 * OVERVIEW:
 *  Executes the 'pb' (push B) instruction. Pops the top node from stack A,
 *  pushes it onto the top of stack B, logs "pb\n" to standard output, and
 *  increments the operation counter.
 *
 * EXECUTION SCHEMA:
 *
 *   config->nums.a == NULL? ───[Yes]───> Return (No-op)
 *             │ [No]
 *             ▼
 *     node = detach_top(&config->nums.a)
 *             │
 *             ▼
 *     attach_top(&config->nums.b, node)
 *             │
 *             ▼
 *     ft_printf("pb\n", 1)
 *     config->metrics.pb++
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *
 * RETURNS:
 *  - void: Modifies stacks A and B, writes output, updates metrics.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe no-op: Returns immediately if stack A is empty without output.
 *  - Preserves node payload without reallocation or memory duplication.
 * =========================================================================
 */
void	pb(t_config *config)
{
	t_stack	*node;

	if (!config->nums.a)
		return ;
	node = detach_top(&config->nums.a);
	attach_top(&config->nums.b, node);
	config->nums.size_a--;
	config->nums.size_b++;
	if (!config->silent)
		ft_printf("pb\n", 1);
	config->metrics.pb++;
	config->metrics.total++;
}
