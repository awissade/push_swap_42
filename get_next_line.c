/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 18:31:42 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/15 09:20:03 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker.h"

void	new_stach(char **stach, size_t i)
{
	char	*tmp;
	size_t	rest;

	rest = ft_strlen((*stach) + i);
	tmp = NULL;
	if (rest > 0)
		tmp = s_alloc((rest + 1) * sizeof(char));
	if (tmp)
		fast_copy(tmp, (*stach) + i, rest + 1);
	*stach = tmp;
}

char	*get_the_line(char **stach)
{
	char	*line;
	size_t	i;
	size_t	j;

	if (!(*stach) || !(**stach))
		return (*stach = NULL, NULL);
	i = 0;
	while ((*stach)[i] && (*stach)[i] != LINE_END)
		i++;
	i += ((*stach)[i] == LINE_END);
	line = s_alloc((i + 1) * sizeof(char));
	if (!line)
		return (*stach = NULL, NULL);
	j = -1;
	while (++j < i)
		line[j] = (*stach)[j];
	line[j] = '\0';
	new_stach(stach, i);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stach;
	char		*buffer;
	ssize_t		readed;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (ft_strchr_ad(stach, LINE_END))
		return (get_the_line(&stach));
	buffer = s_alloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (stach = NULL, NULL);
	readed = read(fd, buffer, BUFFER_SIZE);
	while (readed > 0)
	{
		buffer[readed] = '\0';
		stach = ft_strjoin_ad(&stach, buffer);
		if (!stach || ft_strchr_ad(buffer, LINE_END))
			break ;
		readed = read(fd, buffer, BUFFER_SIZE);
	}
	if (readed == -1)
		return (stach = NULL, NULL);
	return (get_the_line(&stach));
}
