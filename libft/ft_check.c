/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 20:37:29 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:20:09 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_check(char c, int *count, va_list args, t_flags flags)
{
	if (c == 'c')
		ft_pchar(va_arg(args, int), count, flags);
	else if (c == 's')
		ft_pstr(va_arg(args, char *), count, flags);
	else if (c == 'p')
		ft_pptr(va_arg(args, unsigned long), count, flags);
	else if (c == 'd' || c == 'i')
		ft_pnbr(va_arg(args, int), count, flags);
	else if (c == 'u')
		ft_pudec(va_arg(args, unsigned int), count, flags);
	else if (c == 'x')
		ft_phex(va_arg(args, unsigned int), count, "0123456789abcdef", flags);
	else if (c == 'X')
		ft_phex(va_arg(args, unsigned int), count, "0123456789ABCDEF", flags);
	else if (c == '%')
	{
		ft_putchar_fd('%', flags.fd);
		(*count)++;
	}
	else
		return (0);
	return (1);
}
