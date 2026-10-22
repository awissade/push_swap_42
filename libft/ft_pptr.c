/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pptr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 14:24:14 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:21:08 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_phexl(unsigned long nb, int *count, int fd)
{
	char	*base;

	base = "0123456789abcdef";
	if (nb >= 16)
		ft_phexl(nb / 16, count, fd);
	ft_putchar_fd(base[nb % 16], fd);
	(*count)++;
}

static long	calculate_len(unsigned long nb)
{
	int	len;

	if (!nb)
		return (5);
	len = 0;
	while (nb > 0)
	{
		nb /= 16;
		len++;
	}
	return (len + 2);
}

void	ft_pptr(unsigned long nb, int *count, t_flags flags)
{
	int	len;

	len = flags.width - calculate_len(nb);
	if (!flags.minus)
		print_padding(len, count, ' ', flags.fd);
	if (nb)
	{
		ft_putstr_fd("0x", flags.fd);
		(*count) += 2;
		ft_phexl(nb, count, flags.fd);
	}
	else
	{
		ft_putstr_fd("(nil)", flags.fd);
		(*count) += 5;
	}
	if (flags.minus)
		print_padding(len, count, ' ', flags.fd);
}
