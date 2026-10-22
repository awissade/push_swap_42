/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_phex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:41:54 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:20:43 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calculate_len(unsigned int nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		len++;
		nb /= 16;
	}
	return (len);
}

static void	print_number(unsigned int nb, int *count, char *base, int fd)
{
	if (nb >= 16)
		print_number(nb / 16, count, base, fd);
	ft_putchar_fd(base[nb % 16], fd);
	(*count)++;
}

static void	print_perfix(char *base, int *count, int fd)
{
	ft_putchar_fd(base[0], fd);
	ft_putchar_fd(base[10] + 23, fd);
	(*count) += 2;
}

static void	print_content(int *count, t_flags flags, char *base)
{
	if (!flags.minus && !flags.zero)
		print_padding(flags.len_s, count, ' ', flags.fd);
	if (!flags.minus && flags.zero && !flags.dot)
	{
		if (flags.hash && flags.nmb != 0)
			print_perfix(base, count, flags.fd);
		print_padding(flags.len_s, count, '0', flags.fd);
	}
	if (!flags.minus && flags.zero && flags.dot)
		print_padding(flags.len_s, count, ' ', flags.fd);
	if (flags.hash && flags.nmb != 0 && (!flags.zero || flags.dot))
		print_perfix(base, count, flags.fd);
	if (flags.dot)
		print_padding(flags.len_z, count, '0', flags.fd);
	if (!(flags.nmb == 0 && flags.dot && flags.precision == 0))
		print_number(flags.nmb, count, base, flags.fd);
	if (flags.minus)
		print_padding(flags.len_s, count, ' ', flags.fd);
}

void	ft_phex(unsigned int nb, int *count, char *base, t_flags flags)
{
	int	nbr_len;

	flags.nmb = nb;
	nbr_len = calculate_len(nb);
	if (nb == 0 && flags.dot && flags.precision == 0)
		nbr_len = 0;
	flags.len_z = flags.precision - nbr_len;
	if (flags.len_z < 0)
		flags.len_z = 0;
	if (flags.nmb != 0)
		flags.len_s = flags.width - flags.len_z - nbr_len - flags.hash;
	else
		flags.len_s = flags.width - flags.len_z - nbr_len;
	if (flags.len_s < 0)
		flags.len_s = 0;
	print_content(count, flags, base);
}
