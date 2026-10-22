/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_select.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 12:39:07 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 12:39:08 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: dists
 * =========================================================================
 * OVERVIEW:
 *  Measures forward (df) and backward (db) rotational distances from the
 *  top of stack B to a target value using two simultaneous pointers
 *  traversing in opposite directions.
 *
 * EXECUTION SCHEMA:
 *
 *   q (prev) <─── [nums.b] ───> p (next)
 *         │                     │
 *         ↺ Loop: p = p->next, q = q->prev, d++
 *         │
 *         ├── Target found by p first:
 *         │     *df = d
 *         │     *db = (d > 0) * (size_b - d)
 *         │
 *         └── Target found by q first:
 *               *db = d
 *               *df = size_b - d
 *
 * PARAMETERS:
 *  - c {t_config *}: Program context containing stack B and size_b.
 *  - value {int}: Target value to locate in stack B.
 *  - df {int *}: Output pointer for forward distance (rb steps).
 *  - db {int *}: Output pointer for backward distance (rrb steps).
 *
 * RETURNS:
 *  - void: Writes distances into *df and *db in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(min(df, db)) bidirectional search.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - When target is at head (d == 0), setting db = (d > 0) * (size - d)
 *    ensures *db is set to 0 rather than size_b.
 *  - Assumes 'value' exists in stack B to guarantee loop termination.
 * =========================================================================
 */
static void	dists(t_config *c, int value, int *df, int *db)
{
	t_stack	*p;
	t_stack	*q;
	int		d;

	p = c->nums.b;
	q = c->nums.b;
	d = 0;
	while (p->value != value && q->value != value)
	{
		p = p->next;
		q = q->prev;
		d++;
	}
	if (p->value == value)
	{
		*df = d;
		*db = (d > 0) * (c->nums.size_b - d);
	}
	else
	{
		*db = d;
		*df = c->nums.size_b - d;
	}
}

/**
 * =========================================================================
 * FUNCTION: near_dist
 * =========================================================================
 * OVERVIEW:
 *  Queries forward and backward rotational distances to a target value in
 *  stack B and returns the cheaper of the two trajectories.
 *
 * EXECUTION SCHEMA:
 *
 *   value
 *     │
 *     ▼
 *   dists(c, value, &df, &db)
 *     │
 *     ├── df < db ──> Return df (Forward path is shorter)
 *     └── df >= db ─> Return db (Backward path is shorter)
 *
 * PARAMETERS:
 *  - c {t_config *}: Program context containing stack B and metrics.
 *  - value {int}: Target value to evaluate in stack B.
 *
 * RETURNS:
 *  - {int}: Minimal operation count needed to bring 'value' to the top of B.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(min(df, db)) bidirectional search.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Returns 0 immediately if the target is already at the top of stack B.
 * =========================================================================
 */
static int	near_dist(t_config *c, int value)
{
	int	df;
	int	db;

	dists(c, value, &df, &db);
	if (df < db)
		return (df);
	return (db);
}

/**
 * =========================================================================
 * FUNCTION: go_to
 * =========================================================================
 * OVERVIEW:
 *  Rotates stack B via the shortest path until 'value' is at the top.
 *  Opportunistically combines a pending stack A rotation (*pend) with the
 *  first forward rotation of B into a single 'rr' instruction.
 *
 * EXECUTION SCHEMA:
 *
 *   dists(c, value, &df, &db)
 *         │
 *         ├── df <= db && *pend && df > 0?
 *         │         ├── [Yes] ──> rr(c), df--, *pend = 0 (Fused rotation)
 *         │         └── [No]  ──> Continue
 *         │
 *         ├── *pend is still true?
 *         │         ├── [Yes] ──> ra(c), *pend = 0 (Flush deferred ra)
 *         │         └── [No]  ──> Continue
 *         │
 *         ├── df <= db? ──> repeat df times: rb(c)
 *         └── df >  db? ──> repeat db times: rrb(c)
 *
 * PARAMETERS:
 *  - c {t_config *}: Program context containing stacks and metrics.
 *  - value {int}: Value in stack B to rotate to head position.
 *  - pend {bool *}: Deferred 'ra' rotation flag to combine into 'rr'.
 *
 * RETURNS:
 *  - void: Rotates stack B and flushes or updates *pend in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(min(df, db)) emitted rotation instructions.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - If value is already at top, immediately flushes *pend via ra if set.
 *  - Sets *pend to 0 upon completion to prevent duplicate rotations.
 * =========================================================================
 */
static void	go_to(t_config *c, int value, bool *pend)
{
	int	df;
	int	db;

	dists(c, value, &df, &db);
	if (df <= db && *pend && df > 0)
	{
		rr(c);
		df--;
		*pend = 0;
	}
	if (*pend)
		ra(c);
	*pend = 0;
	while (df <= db && df-- > 0)
		rb(c);
	while (df > db && db-- > 0)
		rrb(c);
}

/**
 * =========================================================================
 * FUNCTION: take_one
 * =========================================================================
 * OVERVIEW:
 *  Extracts one extreme element from stack B to stack A. Compares the
 *  cost of extracting the largest remaining element to the top of A versus
 *  the smallest to the bottom of A (+1 cost penalty for deferred ra),
 *  executing the cheaper option.
 *
 * EXECUTION SCHEMA:
 *
 *   Compare costs:
 *   lowest == highest || dist(highest) <= dist(lowest) + 1?
 *        │
 *        ├── [Yes] Extract Maximum:
 *        │     go_to(arr[highest--], &pend)
 *        │     pa(c)  ──> Placed at top of A
 *        │
 *        └── [No]  Extract Minimum:
 *              go_to(arr[lowest++], &pend)
 *              pa(c)  ──> Pushed to top of A
 *              pend = 1, mins++ (Deferred ra to send to bottom)
 *
 * PARAMETERS:
 *  - c {t_config *}: Program context containing stacks and telemetry.
 *  - arr {int *}: Reference sorted subarray of elements in this slice.
 *  - s {t_select *}: Selection state (lowest/highest bounds, mins, pend).
 *
 * RETURNS:
 *  - void: Mutates selection indices, updates pend, and transfers a node.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(size_b) distance checks and rotations.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - When only one element remains (lowest == highest), extracts directly
 *    as maximum to avoid superfluous rotation penalties.
 * =========================================================================
 */
static void	take_one(t_config *c, int *arr, t_select *s)
{
	if (s->lowest == s->highest || near_dist(c, arr[s->highest]) <= near_dist(c,
			arr[s->lowest]) + 1)
	{
		go_to(c, arr[s->highest--], &s->pend);
		pa(c);
	}
	else
	{
		go_to(c, arr[s->lowest++], &s->pend);
		pa(c);
		s->pend = 1;
		s->mins++;
	}
}

/**
 * =========================================================================
 * FUNCTION: select_b
 * =========================================================================
 * OVERVIEW:
 *  Small-slice base-case sorter for stack B (size <= 30). Repeatedly
 *  extracts the closest extreme (minimum or maximum) to stack A until B is
 *  empty, then unwinds all rotated minimums back to the top of A via rra.
 *
 * EXECUTION SCHEMA:
 *
 *   s.lowest = 0, s.highest = size - 1, s.mins = 0, s.pend = 0
 *        │
 *        ↺ Loop while (s.lowest <= s.highest):
 *             └── take_one(config, arr, &s)
 *        │
 *        ├── s.pend is true? ──> s.mins-- (Last min was not rotated past)
 *        │
 *        ▼
 *   Unwind minimums:
 *   while (s.mins-- > 0)
 *        rra(config)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *  - arr {int *}: Sorted subarray corresponding to the slice in B.
 *  - size {int}: Count of elements belonging to this partition.
 *
 * RETURNS:
 *  - void: Empties the target segment from B and places it sorted in A.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(size^2) operations across the extraction passes.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Compensates for un-flushed trailing pending flag before rra unwinding.
 *  - Assumes input subarray arr is sorted in ascending order.
 * =========================================================================
 */
void	select_b(t_config *config, int *arr, int size)
{
	t_select	s;

	s.lowest = 0;
	s.highest = size - 1;
	s.mins = 0;
	s.pend = 0;
	while (s.lowest <= s.highest)
		take_one(config, arr, &s);
	if (s.pend)
		s.mins--;
	while (s.mins-- > 0)
		rra(config);
}
