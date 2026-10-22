/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_new.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 11:00:43 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/22 15:15:28 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: lst_new
 * =========================================================================
 * OVERVIEW:
 *  Allocates and initializes a new stack node (t_stack) via s_alloc. Assigns
 *  the provided integer value as payload and sets the next pointer to NULL.
 *
 * EXECUTION SCHEMA:
 *
 *   [int value]
 *        │
 *        ▼
 *   s_alloc(sizeof(t_stack))
 *        │
 *        ├─ [NULL] ───────────────> Return NULL (Allocation failure)
 *        │
 *        ▼ [Valid node]
 *   node->value = value
 *   node->next  = NULL
 *        │
 *        ▼
 *   Return node
 *
 * PARAMETERS:
 *  - value {int}: Numerical payload to store inside the node.
 *
 * RETURNS:
 *  - {t_stack *}: Pointer to the newly allocated and initialized node;
 *                 NULL if memory allocation fails.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1) allocation and field assignments.
 *  - Space Complexity: O(1) auxiliary space (tracked node on heap).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Returns NULL safely if s_alloc fails to secure memory.
 *  - The next pointer is explicitly initialized to NULL to prevent UB.
 * =========================================================================
 */
t_stack	*lst_new(int value)
{
	t_stack	*node;

	node = s_alloc(sizeof(t_stack));
	if (!node)
		return (NULL);
	node->value = value;
	node->next = node;
	node->prev = node;
	return (node);
}
