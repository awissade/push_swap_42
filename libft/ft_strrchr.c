/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 05:07:23 by abwissad          #+#    #+#             */
/*   Updated: 2026/08/25 05:14:04 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			len;
	unsigned char	target;

	len = 0;
	while (s[len])
		len++;
	target = (unsigned char)c;
	while (1)
	{
		if (s[len] == target)
			return ((char *)&s[len]);
		if (len == 0)
			break ;
		len--;
	}
	return (NULL);
}
