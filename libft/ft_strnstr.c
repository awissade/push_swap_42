/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 06:16:20 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/23 15:39:08 by nychan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *gt, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!little[0])
		return ((char *)gt);
	i = 0;
	while (gt[i] && i < len)
	{
		j = 0;
		while (little[j] && gt[i + j]
			&& (i + j) < len && gt[i + j] == little[j])
		{
			j++;
		}
		if (!little[j])
			return ((char *)&gt[i]);
		i++;
	}
	return (NULL);
}
