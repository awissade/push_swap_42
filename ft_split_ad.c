/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_ad.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 12:15:46 by abwissad          #+#    #+#             */
/*   Updated: 2026/09/23 15:39:16 by nychan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: ft_iswhitespace
 * =========================================================================
 * OVERVIEW:
 *  Determines whether a given character belongs to the standard ASCII
 *  whitespace category (space or control characters 9 through 13).
 *
 * EXECUTION SCHEMA:
 *
 *                char c
 *                  │
 *                  ▼
 *       (c >= 9 && c <= 13) || (c == 32)?
 *             │                 │
 *            Yes                No
 *             │                 │
 *             ▼                 ▼
 *          Return 1          Return 0
 *
 * PARAMETERS:
 *  - c {char}: Character byte to test.
 *
 * RETURNS:
 *  - {int}: 1 if character is a whitespace character; 0 otherwise.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(1).
 *  - Space Complexity: O(1).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Covers horizontal tab ('\t'), newline ('\n'), vertical tab ('\v'),
 *    form feed ('\f'), carriage return ('\r'), and space (' ').
 * =========================================================================
 */
static int	ft_iswhitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

/**
 * =========================================================================
 * FUNCTION: count_words
 * =========================================================================
 * OVERVIEW:
 *  Counts contiguous, non-whitespace substrings within a string. Uses a state
 *  flag (in_word) to detect leading transitions into words.
 *
 * EXECUTION SCHEMA:
 *
 *    s ──> [ ' ', 'H', 'e', 'l', 'l', 'o', ' ', '4', '2', '\0' ]
 *            │    ▲              │    │    ▲
 *            │    └─ !ws: count++│    │    └─ !ws: count++
 *            │       in_word = 1 │    │       in_word = 1
 *            ▼                   ▼    ▼
 *        ws: in_word = 0     ws: in_word = 0
 *
 * PARAMETERS:
 *  - s {char const *}: Null-terminated target string.
 *
 * RETURNS:
 *  - {int}: Total count of delimited words identified.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the length of string 's'.
 *  - Space Complexity: O(1) auxiliary space.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Empty strings or strings containing only whitespace evaluate to 0.
 *  - Multiple consecutive whitespace characters are collapsed cleanly.
 * =========================================================================
 */
static int	count_words(char const *s)
{
	size_t	i;
	int		count;
	int		in_word;

	in_word = 0;
	count = 0;
	i = 0;
	while (s[i])
	{
		if (!ft_iswhitespace(s[i]) && in_word == 0)
		{
			count++;
			in_word = 1;
		}
		if (ft_iswhitespace(s[i]))
			in_word = 0;
		i++;
	}
	return (count);
}

/**
 * =========================================================================
 * FUNCTION: ft_substr_ad
 * =========================================================================
 * OVERVIEW:
 *  Extracts a bounded substring from 's' starting at index 'start' up to
 *  'len' characters. Allocates the destination buffer using s_alloc.
 *
 * EXECUTION SCHEMA:
 *
 *   s, start, len
 *        │
 *        ├── !s ──────────────────────────> Return NULL
 *        │
 *        ▼
 *   Calculate s_len
 *   len = min(len, s_len - start)
 *   ptr = s_alloc(len + 1)
 *        │
 *        ├── [NULL] ──────────────────────> Return NULL (Alloc fail)
 *        │
 *        ▼ [Valid]
 *   Copy characters: ptr[0..len-1] = s[start..start+len-1]
 *   Append terminator: ptr[len] = '\0'
 *   Return ptr
 *
 * PARAMETERS:
 *  - s {char const *}: Source string from which to extract.
 *  - start {unsigned int}: Zero-based start index of substring.
 *  - len {size_t}: Maximum character length of the substring.
 *
 * RETURNS:
 *  - {char *}: Garbage-collector tracked pointer to the new substring;
 *              NULL if 's' is NULL or memory allocation fails.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N + len), where N is strlen(s).
 *  - Space Complexity: O(len) tracked heap memory for the substring.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Clamps 'len' to prevent reading beyond the null terminator.
 *  - Assumes 'start' does not exceed 's_len' to prevent unsigned underflow.
 * =========================================================================
 */
static char	*ft_substr_ad(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*ptr;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (len > s_len - start)
		len = s_len - start;
	ptr = s_alloc((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[start + i] && i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

/**
 * =========================================================================
 * FUNCTION: fill_array
 * =========================================================================
 * OVERVIEW:
 *  Iterates across string 's', discovers boundaries of non-whitespace words,
 *  allocates each slice via ft_substr_ad, and populates the pointers of 'arr'.
 *
 * EXECUTION SCHEMA:
 *
 *   arr [ ? ][ ? ][ ? ]   s ──> "  abc   def  "
 *                                  │     │
 *         ┌────────────────────────┘     │
 *         ▼                              ▼
 *   ft_substr_ad("abc", 3)         ft_substr_ad("def", 3)
 *         │                              │
 *         ▼                              ▼
 *   arr [ "abc" ][ "def" ][ NULL ]
 *
 * PARAMETERS:
 *  - arr {char **}: Preallocated array of string pointers.
 *  - s {char const *}: Null-terminated source string to parse.
 *
 * RETURNS:
 *  - {char **}: The populated array 'arr' terminated with a NULL pointer;
 *               NULL if any substring allocation fails.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the length of string 's'.
 *  - Space Complexity: O(1) auxiliary space (excluding allocations).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Skips arbitrary sequences of leading, trailing, and middle spaces.
 *  - Returns NULL immediately if ft_substr_ad returns NULL on any token.
 * =========================================================================
 */
static char	**fill_array(char **arr, char const *s)
{
	size_t	word_len;
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (!ft_iswhitespace(s[i]))
		{
			word_len = 0;
			while (s[i + word_len] && !ft_iswhitespace(s[i + word_len]))
				word_len++;
			arr[j] = ft_substr_ad(s, i, word_len);
			if (!arr[j])
				return (NULL);
			j++;
			i += word_len;
		}
		else
			i++;
	}
	arr[j] = NULL;
	return (arr);
}

/**
 * =========================================================================
 * FUNCTION: ft_split_ad
 * =========================================================================
 * OVERVIEW:
 *  Splits string 's' across all ASCII whitespace delimiters into an array
 *  of words. Uses s_alloc to secure garbage-collector tracked allocations.
 *
 * EXECUTION SCHEMA:
 *
 *   s (input string)
 *        │
 *        ├── !s ───────────────────────> Return NULL
 *        │
 *        ▼
 *   words = count_words(s)
 *   arr = s_alloc((words + 1) * sizeof(char *))
 *        │
 *        ├── [NULL] ───────────────────> Return NULL (Alloc fail)
 *        │
 *        ▼ [Valid]
 *   fill_array(arr, s) ────────────────> Populate tokens & NULL terminator
 *        │
 *        ▼
 *   Return arr
 *
 * PARAMETERS:
 *  - s {char const *}: Null-terminated string to split.
 *
 * RETURNS:
 *  - {char **}: Array of word strings terminated by NULL;
 *               NULL if 's' is NULL or allocation fails.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N), where N is the character length of 's'.
 *  - Space Complexity: O(N) heap memory tracked by the garbage collector.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Returns NULL if input pointer 's' is NULL.
 *  - An empty or whitespace-only string returns an array with a single NULL.
 * =========================================================================
 */
char	**ft_split_ad(char const *s)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = s_alloc((count_words(s) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	return (fill_array(arr, s));
}
