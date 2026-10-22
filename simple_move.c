/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 13:23:35 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 13:29:33 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: try_way
 * =========================================================================
 * OVERVIEW:
 *  Calculates the total move cost of a rotation pair (x for A, y for B).
 *  Optimizes for common signs by taking max(|x|, |y|) when both can share
 *  double rotations (rr or rrr); otherwise sums absolute steps |x| + |y|.
 *  Updates best move record if candidate cost is strictly lower.
 *
 * EXECUTION SCHEMA:
 *
 *   Inputs: x (A rotations), y (B rotations)
 *        │
 *        ▼
 *   ax = |x|, ay = |y|
 *        │
 *        ├── Same direction? ((x >= 0 && y >= 0) || (x <= 0 && y <= 0))
 *        │         ├── [Yes] ──> cost = max(ax, ay)  [Merged rr / rrr]
 *        │         └── [No]  ──> cost = ax + ay      [Independent ops]
 *        │
 *        ▼
 *   cost < best->cost?
 *        ├── [Yes] ────────────> best->cost = cost, best->x = x, best->y = y
 *        └── [No]  ────────────> Discard candidate
 *
 * PARAMETERS:
 *  - best {t_move *}: Tracker updated when a cheaper path is found.
 *  - x {int}: Signed rotations for stack A (+ for ra, - for rra).
 *  - y {int}: Signed rotations for stack B (+ for rb, - for rrb).
 *
 * RETURNS:
 *  - void: Mutates best struct in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Correctly evaluates zero rotations without unnecessary penalties.
 *  - Assumes best->cost was initialized before first call (e.g., INT_MAX).
 * =========================================================================
 */
static void	try_way(t_move *best, int x, int y)
{
	int	cost;
	int	ax;
	int	ay;

	ax = x;
	if (ax < 0)
		ax = -x;
	ay = y;
	if (ay < 0)
		ay = -ay;
	cost = ax + ay;
	if ((x >= 0 && y >= 0) || (x <= 0 && y <= 0))
	{
		cost = ax;
		if (ay > ax)
			cost = ay;
	}
	if (cost < best->cost)
	{
		best->cost = cost;
		best->x = x;
		best->y = y;
	}
}

/**
 * =========================================================================
 * FUNCTION: best_way
 * =========================================================================
 * OVERVIEW:
 *  Evaluates all four rotation strategies to align a pair of elements
 *  between stacks A and B (forward-forward, backward-backward, and mixed).
 *  Determines the strategy with the minimal overall instruction count.
 *
 * EXECUTION SCHEMA:
 *
 *   af (A forward), bf (B forward)
 *        │
 *        ▼
 *   ab = (af > 0) ? size_a - af : 0   [A backward steps]
 *   bb = (bf > 0) ? size_b - bf : 0   [B backward steps]
 *   move->cost = INT_MAX
 *        │
 *        ├─ 1. try_way(move,  af,  bf)   [Both forward: rr]
 *        ├─ 2. try_way(move, -ab, -bb)   [Both backward: rrr]
 *        ├─ 3. try_way(move,  af, -bb)   [A forward, B backward]
 *        └─ 4. try_way(move, -ab,  bf)   [A backward, B forward]
 *        │
 *        ▼
 *   Optimal trajectory stored in move
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing size_a and size_b.
 *  - af {int}: Forward distance from top of stack A (>= 0).
 *  - bf {int}: Forward distance from top of stack B (>= 0).
 *  - move {t_move *}: Move descriptor storing optimal x, y, and cost.
 *
 * RETURNS:
 *  - void: Populates move with the cheapest rotation combination.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Prevents underflow or invalid backward steps when af == 0 or bf == 0.
 *  - Assumes size_a and size_b are accurate reflections of current stacks.
 * =========================================================================
 */
void	best_way(t_config *config, int af, int bf, t_move *move)
{
	int	ab;
	int	bb;

	ab = 0;
	if (af > 0)
		ab = config->nums.size_a - af;
	bb = 0;
	if (bf > 0)
		bb = config->nums.size_b - bf;
	move->cost = INT_MAX;
	try_way(move, af, bf);
	try_way(move, -ab, -bb);
	try_way(move, af, -bb);
	try_way(move, -ab, bf);
}

/**
 * =========================================================================
 * FUNCTION: rotate_together
 * =========================================================================
 * OVERVIEW:
 *  Executes simultaneous rotations (rr or rrr) on stacks A and B while
 *  both remaining step counters share the same direction. Decrements or
 *  increments the counter references until at least one reaches zero.
 *
 * EXECUTION SCHEMA:
 *
 *   *x > 0 && *y > 0? (Both need forward rotation)
 *        │
 *        ↺ Loop: rr(c), (*x)--, (*y)--
 *
 *   *x < 0 && *y < 0? (Both need reverse rotation)
 *        │
 *        ↺ Loop: rrr(c), (*x)++, (*y)++
 *
 * PARAMETERS:
 *  - c {t_config *}: Context containing stacks and metrics.
 *  - x {int *}: Pointer to stack A remaining rotation count.
 *  - y {int *}: Pointer to stack B remaining rotation count.
 *
 * RETURNS:
 *  - void: Emits instructions and mutates counters *x and *y in place.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(min(|x|, |y|)) emitted joint operations.
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - If signs differ (*x > 0 && *y < 0 or vice versa), performs 0 moves.
 *  - Consumes joint overlap completely, leaving at least one counter at 0.
 * =========================================================================
 */
static void	rotate_together(t_config *c, int *x, int *y)
{
	while (*x > 0 && *y > 0)
	{
		rr(c);
		(*x)--;
		(*y)--;
	}
	while (*x < 0 && *y < 0)
	{
		rrr(c);
		(*x)++;
		(*y)++;
	}
}

/**
 * =========================================================================
 * FUNCTION: rotate_pair
 * =========================================================================
 * OVERVIEW:
 *  Applies the complete rotation movement for both stacks. First consumes
 *  overlapping directions using simultaneous double rotations (rr / rrr),
 *  then exhausts any remaining individual steps using ra, rra, rb, or rrb.
 *
 * EXECUTION SCHEMA:
 *
 *   x (A steps), y (B steps)
 *        │
 *        ▼
 *   rotate_together(config, &x, &y) ──> Consume joint rr or rrr
 *        │
 *        ├─ x > 0: while (x--) ra(config)
 *        ├─ x < 0: while (x++) rra(config)
 *        ├─ y > 0: while (y--) rb(config)
 *        └─ y < 0: while (y++) rrb(config)
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stacks and metrics.
 *  - x {int}: Signed rotation count for stack A.
 *  - y {int}: Signed rotation count for stack B.
 *
 * RETURNS:
 *  - void: Modifies stack head positions and emits instructions.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(max(|x|, |y|)) total rotation instructions.
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Non-interfering: Zero step values bypass loop bodies cleanly.
 *  - Guarantees exact final alignment of both stacks to designated targets.
 * =========================================================================
 */
void	rotate_pair(t_config *config, int x, int y)
{
	rotate_together(config, &x, &y);
	while (x > 0)
	{
		ra(config);
		x--;
	}
	while (x < 0)
	{
		rra(config);
		x++;
	}
	while (y > 0)
	{
		rb(config);
		y--;
	}
	while (y < 0)
	{
		rrb(config);
		y++;
	}
}

/**
 * =========================================================================
 * FUNCTION: rotate_a
 * =========================================================================
 * OVERVIEW:
 *  Brings a target element in stack A to the top using the shortest
 *  rotational trajectory. Chooses between forward rotations (ra) and
 *  reverse rotations (rra) by comparing distance from top versus bottom.
 *
 * EXECUTION SCHEMA:
 *
 *   steps (forward index from head)
 *        │
 *        ▼
 *   steps <= (size_a - steps)?
 *        ├── [Yes] ──> rotate_pair(config, steps, 0)
 *        │             [Forward: ra * steps]
 *        │
 *        └── [No]  ──> rotate_pair(config, steps - size_a, 0)
 *                      [Reverse: rra * (size_a - steps)]
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing size_a.
 *  - steps {int}: Zero-based forward position of desired element in A.
 *
 * RETURNS:
 *  - void: Rotates stack A until the target node becomes the head.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(min(steps, size_a - steps)) rotations.
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - If steps == 0, evaluates as 0 moves and performs no rotations.
 *  - Relies on config->nums.size_a matching the current node count of A.
 * =========================================================================
 */
void	rotate_a(t_config *config, int steps)
{
	if (steps <= config->nums.size_a - steps)
		rotate_pair(config, steps, 0);
	else
		rotate_pair(config, steps - config->nums.size_a, 0);
}
