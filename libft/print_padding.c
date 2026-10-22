/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_padding.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 09:16:56 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/12 11:21:46 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_padding(int len, int *count, char c, int fd)
{
	while (len > 0)
	{
		ft_putchar_fd(c, fd);
		(*count)++;
		len--;
	}
}
