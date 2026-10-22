/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstadd_back.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 11:05:19 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/22 11:17:07 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: lstadd_back
 * =========================================================================
 * OVERVIEW:
 *  Appends a node to the tail of the singly-linked stack. If the stack is
 *  empty, sets the node directly as the new head.
 *
 * EXECUTION SCHEMA:
 *
 *   !a || !new? ────────[Yes]───────> Return (No-op guard)
 *        │ [No]
 *        ▼
 *    *a == NULL? ───────[Yes]───────> *a = new (Set as first element)
 *        │ [No]                       Return
 *        ▼
 *   Traverse to tail:
 *   last = *a
 *   while (last->next)
 *       last = last->next
 *        │
 *        ▼
 *   last->next = new (Link to tail)
 *
 * PARAMETERS:
 *  - a {t_stack **}: Address of the head pointer of the target stack.
 *  - new {t_stack *}: Node to be appended to the end of the stack.
 *
 * RETURNS:
 *  - void: Modifies the list structure in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the current number of nodes in 'a'.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Safe against NULL parameters: returns early if either 'a' or 'new' is NULL.
 *  - If '*a' is NULL (empty list), safely initializes the list root to 'new'.
 * =========================================================================
 */
void	lstadd_back(t_stack **a, t_stack *new)
{
	t_stack	*tail;

	if (!a || !new)
		return ;
	if (!*a)
	{
		*a = new;
		return ;
	}
	tail = (*a)->prev;
	new->next = (*a);
	new->prev = tail;
	tail->next = new;
	(*a)->prev = new;
}
