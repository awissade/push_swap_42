/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 10:02:25 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/08 09:23:15 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: print_strategy
 * =========================================================================
 * OVERVIEW:
 *  Outputs the configured sorting strategy name (strat) alongside the
 *  theoretical time complexity of the effectively executed strategy
 *  (chosen) to standard error (fd 2).
 *
 * EXECUTION SCHEMA:
 *
 *   config->strat
 *        ├── SIMPLE   ──> "[bench] strategy: Simple / "
 *        ├── MEDIUM   ──> "[bench] strategy: Medium / "
 *        ├── COMPLEX  ──> "[bench] strategy: Complex / "
 *        └── (Else)   ──> "[bench] strategy: Adaptive / "
 *                 │
 *                 ▼
 *   config->chosen
 *        ├── SIMPLE   ──> "O(n2)\n"
 *        ├── MEDIUM   ──> "O(n√n)\n"
 *        └── COMPLEX  ──> "O(n log n)\n"
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing requested strategy
 *                         (strat) and executed strategy (chosen).
 *
 * RETURNS:
 *  - void: Emits formatted text directly to stderr (fd 2).
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Writes exclusively to file descriptor 2 (standard error).
 *  - Assumes config->chosen is set to SIMPLE, MEDIUM, or COMPLEX to ensure
 *    the complexity tier and trailing newline are emitted.
 * =========================================================================
 */
static void	print_strategy(t_config *config)
{
	if (config->strat == SIMPLE)
		ft_printf("[bench] strategy: Simple / ", 2);
	else if (config->strat == MEDIUM)
		ft_printf("[bench] strategy: Medium / ", 2);
	else if (config->strat == COMPLEX)
		ft_printf("[bench] strategy: Complex / ", 2);
	else
		ft_printf("[bench] strategy: Adaptive / ", 2);
	if (config->chosen == SIMPLE)
		ft_printf("O(n^2)\n", 2);
	if (config->chosen == MEDIUM)
		ft_printf("O(n√n)\n", 2);
	if (config->chosen == COMPLEX)
		ft_printf("O(n log n)\n", 2);
}

/**
 * =========================================================================
 * FUNCTION: bench
 * =========================================================================
 * OVERVIEW:
 *  Emits complete benchmark diagnostics to standard error (fd 2). Prints
 *  the input disorder score as a two-decimal percentage, delegates strategy
 *  logging, and displays an itemized tally of every executed push_swap
 *  instruction alongside the grand total.
 *
 * EXECUTION SCHEMA:
 *
 *   1. Format disorder metric:
 *      (disorder * 100) . ((disorder * 10000) % 100)
 *      └──> "[bench] disorder: X.Y%\n"
 *               │
 *               ▼
 *   2. print_strategy(config)
 *      └──> "[bench] strategy: <name> / <complexity>\n"
 *               │
 *               ▼
 *   3. Operation Telemetry:
 *      ├──> "[bench] total_ops: <total>\n"
 *      ├──> "[bench] sa: .. sb: .. ss: .. pa: .. pb: ..\n"
 *      └──> "[bench] ra: .. rb: .. rr: .. rra: .. rrb: .. rrr: ..\n"
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing execution metrics,
 *                         disorder score, and strategy configuration.
 *
 * RETURNS:
 *  - void: Outputs diagnostic telemetry to stderr (fd 2).
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1) auxiliary space (local metrics struct copy).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Uses integer arithmetic to format two decimal places without
 *    relying on floating-point format specifiers in ft_printf.
 *  - Keeps diagnostic logs separated on stderr so redirected stdout
 *    instruction streams remain clean for the checker.
 * =========================================================================
 */
void	bench(t_config *config)
{
	int		pct;
	t_bench	n;

	n = config->metrics;
	pct = (int)(config->disorder * 10000);
	ft_printf("[bench] disorder: %d.%02d%%\n", 2, pct / 100, pct % 100);
	print_strategy(config);
	ft_printf("[bench] total_ops: %d\n", 2, n.total);
	ft_printf("[bench] sa: %d sb: %d ss: %d pa: %d pb: %d\n", 2,
		n.sa, n.sb, n.ss, n.pa, n.pb);
	ft_printf("[bench] ra: %d rb: %d rr: %d rra: %d rrb: %d rrr: %d\n", 2,
		n.ra, n.rb, n.rr, n.rra, n.rrb, n.rrr);
}
