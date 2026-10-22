/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 01:29:25 by abwissad          #+#    #+#             */
/*   Updated: 2026/08/26 02:55:09 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	size_t	i;
	int		count;
	int		in_word;

	in_word = 0;
	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && in_word == 0)
		{
			count++;
			in_word = 1;
		}
		if (s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static char	**free_array(char **arr, int j)
{
	while (j >= 0)
	{
		free(arr[j]);
		j--;
	}
	free(arr);
	return (NULL);
}

static char	**fill_array(char **arr, char const *s, char c)
{
	size_t	word_len;
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			word_len = 0;
			while (s[i + word_len] && s[i + word_len] != c)
				word_len++;
			arr[j] = ft_substr(s, i, word_len);
			if (!arr[j])
				return (free_array(arr, j - 1));
			j++;
			i += word_len;
		}
		else
			i++;
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	return (fill_array(arr, s, c));
}
