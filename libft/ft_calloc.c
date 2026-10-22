/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 17:35:06 by abwissad          #+#    #+#             */
/*   Updated: 2026/08/25 20:51:55 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_memset_help(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n > 0)
	{
		*ptr = (unsigned char)c;
		n--;
		ptr++;
	}
	return (s);
}

void	*ft_calloc(size_t nbr, size_t size)
{
	void	*ptr;

	if (size != 0 && nbr > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(nbr * size);
	if (!ptr)
		return (NULL);
	return (ft_memset_help(ptr, 0, nbr * size));
}
