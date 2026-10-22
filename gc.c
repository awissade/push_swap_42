/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 16:03:14 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/23 14:23:13 by nychan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * ============================================================================
 * FUNCTION: get_head
 * ============================================================================
 * OVERVIEW:
 *  Provides a centralized, persistent access point (Singleton pattern) to the
 *  garbage collector list root. Encapsulates a static list pointer, preserving
 *  allocation records across distinct calls and translation units without
 *  exposing mutable global variables to the broader namespace.
 *
 * SCHEMA / DATA FLOW:
 *
 *   Caller (s_alloc / g_free)
 *             │
 *             ▼
 *        get_head()
 *             │
 *             ▼
 *    ┌───────────────────┐
 *    │   Static Memory   │
 *    │   (BSS Segment)   │
 *    ├───────────────────┤
 *    │   t_list *head    │ ──────> [Node 1] ──> [Node 2] ──> NULL
 *    └─────────┬─────────┘
 *              │
 *              └─────────> Returns &head (t_list **)
 *
 * PARAMETERS:
 *  - void: Operates without parameters.
 *
 * RETURNS:
 *  - {t_list **}: Direct address of the internal static head pointer, allowing
 *                 callers to read, re-link, or reset the root of the tracker.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1)
 *  - Space Complexity: O(1)
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Lifecycle: `head` initializes to NULL once on program startup in BSS.
 *  - Concurrency: Not thread-safe; intended for single-threaded runtime cycles.
 * ============================================================================
 */
t_list	**get_head(void)
{
	static t_list	*head;

	return (&head);
}

/**
 * ============================================================================
 * FUNCTION: s_alloc
 * ============================================================================
 * OVERVIEW:
 *  Safe heap allocator with automatic registration. Allocates a memory block
 *  of the specified size and immediately enqueues its address into the global
 *  collector list, offloading manual deallocation lifecycles from the caller.
 *
 * EXECUTION SCHEMA:
 *
 *             [size_t size]
 *                   │
 *                   ▼
 *             malloc(size)
 *                   │
 *         ┌─────────┴─────────┐
 *       [NULL]           [Valid ptr]
 *         │                   │
 *    Return NULL              ▼
 *                      ft_lstnew(ptr)
 *                             │
 *                   ┌─────────┴─────────┐
 *                 [NULL]            [Valid node]
 *                   │                   │
 *              Return NULL*             ▼
 *                               ft_lstadd_back(&head, node)
 *                                       │
 *                                       ▼
 *                                   Return ptr
 *
 * PARAMETERS:
 *  - size {size_t}: Number of bytes to allocate on the heap.
 *
 * RETURNS:
 *  - {void *}: Address of the allocated memory on success.
 *  - {NULL}  : If either `malloc` or node encapsulation fails.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N) where N is the current number of tracked nodes,
 *                      due to the linear tail traversal in `ft_lstadd_back`.
 *  - Space Complexity: O(1) auxiliary per call beyond the requested bytes and
 *                      the internal `sizeof(t_list)` node metadata.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - System Out-of-Memory: Returns NULL safely if `malloc` fails.
 *  - *Orphan Leak Risk: If `malloc` succeeds but `ft_lstnew` fails, `ptr` is
 *    never added to the list and remains un-freed before returning NULL.
 * ============================================================================
 */
void	*s_alloc(size_t size)
{
	void		*ptr;
	t_list		*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = ft_lstnew(ptr);
	if (!node)
		return (NULL);
	ft_lstadd_back(get_head(), node);
	return (ptr);
}

/**
 * =========================================================================
 * FUNCTION: g_free
 * =========================================================================
 * OVERVIEW:
 *  Full-sweep garbage collector deallocator. Traverses the internal tracked
 *  list, releases each user buffer via standard free, destroys each list
 *  node, and resets the static root pointer to NULL for future cycles.
 *
 * EXECUTION SCHEMA:
 *
 *   get_head() ──> &head
 *                    │
 *                    ▼
 *              *garbage != NULL?
 *               ├─── No  ───> Exit (List is already empty)
 *               │
 *               └─── Yes ───> tmp = *garbage
 *                             *garbage = (*garbage)->next
 *                                    │
 *                                    ▼
 *                             ft_lstdelone(tmp, free)
 *                                    │
 *                                    ├─> free(tmp->content)  [Buffer]
 *                                    └─> free(tmp)           [Node]
 *                                    │
 *                                    ↺ (Repeat until *garbage == NULL)
 *
 * PARAMETERS:
 *  - void: Accesses the internal list directly through get_head().
 *
 * RETURNS:
 *  - void: Mutates and resets collector state in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the total count of allocations.
 *  - Space Complexity: O(1) auxiliary memory.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Empty Collector: Safe against NULL dereferences if invoked when no
 *    allocations are active (*garbage == NULL).
 *  - Post-Execution State: Resets *garbage (head) to NULL, preventing
 *    dangling pointer references across subsequent calls.
 * =========================================================================
 */
void	g_free(void)
{
	t_list	**garbage;
	t_list	*tmp;

	garbage = get_head();
	while (*garbage)
	{
		tmp = (*garbage);
		(*garbage) = (*garbage)->next;
		ft_lstdelone(tmp, free);
	}
}
