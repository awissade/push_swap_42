/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 04:14:42 by abwissad          #+#    #+#             */
/*   Updated: 2026/08/25 04:27:20 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	i;

	src_len = 0;
	while (src[src_len])
		src_len++;
	dst_len = 0;
	while (dest[dst_len] && dst_len < size)
		dst_len++;
	if (dst_len == size)
		return (dst_len + src_len);
	i = 0;
	while (src[i] && (dst_len + i) < (size - 1))
	{
		dest[dst_len + i] = src[i];
		i++;
	}
	dest[dst_len + i] = '\0';
	return (dst_len + src_len);
}
