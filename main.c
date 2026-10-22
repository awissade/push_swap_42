/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:32:35 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/17 14:32:41 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int ac, char **av)
{
	t_config	config;

	if (ac < 2)
		return (1);
	ft_memset(&config, 0, sizeof(config));
	if (!parse_input(ac, av, &config))
	{
		ft_printf("Error\n", 2);
		g_free();
		return (1);
	}
	config.disorder = calculate_disorder(&config);
	if (config.strat == SIMPLE)
		simple(&config);
	else if (config.strat == MEDIUM)
		medium(&config);
	else if (config.strat == COMPLEX)
		complex(&config);
	else
		adaptive(&config);
	if (config.bench)
		bench(&config);
	g_free();
	return (0);
}
