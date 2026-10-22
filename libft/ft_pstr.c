/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pstr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:39:02 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:21:24 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_pstr(char *str, int *count, t_flags flags)
{
	size_t	i;
	int		len_s;
	size_t	str_len;

	if (!str)
	{
		str = "(null)";
		if (flags.dot && flags.precision < 6)
			str = "";
	}
	str_len = ft_strlen(str);
	if (flags.dot && (size_t)flags.precision < str_len)
		str_len = flags.precision;
	len_s = flags.width - str_len;
	if (!flags.minus)
		print_padding(len_s, count, ' ', flags.fd);
	i = 0;
	while (i < str_len)
	{
		ft_putchar_fd(str[i], flags.fd);
		(*count)++;
		i++;
	}
	if (flags.minus)
		print_padding(len_s, count, ' ', flags.fd);
}
