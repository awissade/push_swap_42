/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pchar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:40:17 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:20:35 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_pchar(char c, int *count, t_flags flags)
{
	if (!flags.minus)
		print_padding(flags.width - 1, count, ' ', flags.fd);
	ft_putchar_fd(c, flags.fd);
	(*count)++;
	if (flags.minus)
		print_padding(flags.width - 1, count, ' ', flags.fd);
}
