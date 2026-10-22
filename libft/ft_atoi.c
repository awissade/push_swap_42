/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 06:30:21 by abwissad          #+#    #+#             */
/*   Updated: 2026/08/25 17:28:47 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_spaces(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static int	ft_digits(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *ptr)
{
	int	sign;
	int	total;

	sign = 1;
	while (ft_spaces(*ptr))
		ptr++;
	if (*ptr == '-' || *ptr == '+')
	{
		if (*ptr == '-')
			sign = -1;
		ptr++;
	}
	total = 0;
	while (ft_digits(*ptr))
	{
		total = (total * 10) + (*ptr - '0');
		ptr++;
	}
	return (total * sign);
}
