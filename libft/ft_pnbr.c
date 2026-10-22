/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pnbr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:41:12 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:20:56 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_num_len(long nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

static void	print_sign(int n, int *count, t_flags flags)
{
	if (n < 0)
	{
		ft_putchar_fd('-', flags.fd);
		(*count)++;
	}
	else if (flags.plus)
	{
		ft_putchar_fd('+', flags.fd);
		(*count)++;
	}
	else if (flags.space)
	{
		ft_putchar_fd(' ', flags.fd);
		(*count)++;
	}
}

static void	print_number(long nb, int *count, int fd)
{
	char	c;

	if (nb >= 10)
		print_number(nb / 10, count, fd);
	c = '0' + (nb % 10);
	ft_putchar_fd(c, fd);
	(*count)++;
}

static void	print_nbr_content(int len_s, int len_z, int *count, t_flags flags)
{
	if (!flags.minus && !flags.zero)
		print_padding(len_s, count, ' ', flags.fd);
	print_sign(flags.n, count, flags);
	if (!flags.minus && flags.zero)
	{
		if (!flags.dot)
			print_padding(len_s, count, '0', flags.fd);
		if (flags.dot)
			print_padding(len_s, count, ' ', flags.fd);
	}
	if (flags.dot)
		print_padding(len_z, count, '0', flags.fd);
	if (!(flags.n == 0 && flags.dot && flags.precision == 0))
		print_number(flags.nb, count, flags.fd);
	if (flags.minus)
		print_padding(len_s, count, ' ', flags.fd);
}

void	ft_pnbr(int n, int *count, t_flags flags)
{
	int		sign_len;
	int		nbr_len;
	int		len_z;
	int		len_s;

	flags.nb = n;
	flags.n = n;
	if (flags.nb < 0)
		flags.nb = -flags.nb;
	sign_len = (flags.n < 0 || flags.space || flags.plus);
	nbr_len = get_num_len(flags.nb);
	if (n == 0 && flags.dot && flags.precision == 0)
		nbr_len = 0;
	len_z = flags.precision - nbr_len;
	if (len_z < 0)
		len_z = 0;
	len_s = flags.width - sign_len - len_z - nbr_len;
	if (len_s < 0)
		len_s = 0;
	print_nbr_content(len_s, len_z, count, flags);
}
