/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pudec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:25:24 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:21:35 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_number(unsigned int nb, int *count, int fd)
{
	char	c;

	if (nb >= 10)
		print_number(nb / 10, count, fd);
	c = '0' + (nb % 10);
	ft_putchar_fd(c, fd);
	(*count)++;
}

static int	calculate_len(unsigned int nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

static void	print_nb_content(int len_s, int len_z, int *count, t_flags flags)
{
	if (!flags.minus && !flags.zero)
		print_padding(len_s, count, ' ', flags.fd);
	if (!flags.minus && flags.zero)
	{
		if (!flags.dot)
			print_padding(len_s, count, '0', flags.fd);
		if (flags.dot)
			print_padding(len_s, count, ' ', flags.fd);
	}
	if (flags.dot)
		print_padding(len_z, count, '0', flags.fd);
	if (!(flags.nmb == 0 && flags.dot && flags.precision == 0))
		print_number(flags.nmb, count, flags.fd);
	if (flags.minus)
		print_padding(len_s, count, ' ', flags.fd);
}

void	ft_pudec(unsigned int nb, int *count, t_flags flags)
{
	int	len_s;
	int	len_z;
	int	nbr_len;

	flags.nmb = nb;
	nbr_len = calculate_len(nb);
	len_s = 0;
	if (nb == 0 && flags.dot && flags.precision == 0)
		nbr_len = 0;
	len_z = flags.precision - nbr_len;
	if (len_z < 0)
		len_z = 0;
	len_s = flags.width - len_z - nbr_len;
	if (len_s < 0)
		len_s = 0;
	print_nb_content(len_s, len_z, count, flags);
}
