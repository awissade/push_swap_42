/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:42:42 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/16 11:56:34 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker.h"

static size_t	has_zero_byte(size_t v)
{
	size_t	ones;
	size_t	highs;

	ones = (size_t)-1 / 0xFF;
	highs = ones * 0x80;
	return ((v - ones) & ~v & highs);
}

bool	ft_strchr_ad(char *str, char c)
{
	size_t	i;

	i = 0;
	if (!str)
		return (false);
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (true);
	return (false);
}

size_t	ft_strlen_ad(const char *str)
{
	const char		*start;
	const size_t	*w;

	if (!str)
		return (0);
	start = str;
	while ((uintptr_t)str % sizeof(size_t))
	{
		if (!*str)
			return (str - start);
		str++;
	}
	w = (const size_t *)str;
	while (!has_zero_byte(*w))
		w++;
	str = (const char *)w;
	while (*str)
		str++;
	return (str - start);
}

void	fast_copy(char *dst, char *src, size_t len)
{
	size_t	copied;

	copied = 0;
	while (len - copied >= sizeof(size_t))
	{
		*(size_t *)(dst + copied) = *(size_t *)(src + copied);
		copied += sizeof(size_t);
	}
	while (copied < len)
	{
		dst[copied] = src[copied];
		copied++;
	}
}

char	*ft_strjoin_ad(char **str1, char *str2)
{
	size_t	len1;
	size_t	len2;
	char	*new;

	len1 = ft_strlen_ad(*str1);
	len2 = ft_strlen_ad(str2);
	new = s_alloc((len1 + len2 + 1) * sizeof(char));
	if (!new)
		return (*str1 = NULL, NULL);
	fast_copy(new, *str1, len1);
	fast_copy(new + len1, str2, len2);
	new[len1 + len2] = '\0';
	return (*str1 = NULL, new);
}
