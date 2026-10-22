/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 05:52:33 by abwissad          #+#    #+#             */
/*   Updated: 2026/08/25 06:00:07 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	unsigned char		target;

	ptr = (const unsigned char *)s;
	target = (unsigned char)c;
	while (n > 0)
	{
		if (*ptr == target)
			return ((void *)ptr);
		ptr++;
		n--;
	}
	return (NULL);
}
