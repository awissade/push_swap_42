/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 03:06:02 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:21:16 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_atoi_print(const char *str, size_t *i)
{
	int	res;

	res = 0;
	while (ft_isdigit(str[*i]))
	{
		res = (res * 10) + (str[*i] - '0');
		(*i)++;
	}
	return (res);
}

static void	parse_precision(const char *str, size_t *i, t_flags *flags)
{
	if (str[*i] == '.')
	{
		flags->dot = 1;
		(*i)++;
		if (ft_isdigit(str[*i]))
			flags->precision = ft_atoi_print(str, i);
	}
	if (flags->minus)
		flags->zero = 0;
	if (flags->plus)
		flags->space = 0;
}

static int	check_flags(const char *str, size_t *i, t_flags *flags)
{
	ft_memset(flags, 0, sizeof(t_flags) - sizeof(int));
	(*i)++;
	while (str[*i] && !ft_strchr("cspdiuxX%.", str[*i]))
	{
		if (!ft_strchr("-0.# +", str[*i]) && !ft_isdigit(str[*i]))
			return (0);
		if (str[*i] == '-')
			flags->minus = 1;
		else if (str[*i] == '0')
			flags->zero = 1;
		else if (str[*i] == '#')
			flags->hash = 2;
		else if (str[*i] == ' ')
			flags->space = 1;
		else if (str[*i] == '+')
			flags->plus = 1;
		else if (ft_isdigit(str[*i]))
		{
			flags->width = ft_atoi_print(str, i);
			continue ;
		}
		(*i)++;
	}
	parse_precision(str, i, flags);
	return (1);
}

static int	printing_str(const char *str, va_list args)
{
	size_t	i;
	int		count;
	t_flags	flags;

	i = -1;
	count = 0;
	flags.fd = va_arg(args, int);
	while (str[++i])
	{
		if (str[i] == '%')
		{
			if (str[i + 1] == '\0')
				return (-1);
			if (!check_flags(str, &i, &flags))
				return (-1);
			if (!ft_check(str[i], &count, args, flags))
				return (-1);
		}
		else
		{
			ft_putchar_fd(str[i], flags.fd);
			count++;
		}
	}
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;

	if (!str)
		return (-1);
	va_start(args, str);
	count = printing_str(str, args);
	va_end(args);
	return (count);
}
