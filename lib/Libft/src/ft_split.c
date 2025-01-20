/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/11 12:58:25 by ashevchu      #+#    #+#                 */
/*   Updated: 2023/03/18 20:59:36 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static	size_t	word_count(char *s, char set)
{
	size_t	parts;
	int		found;

	parts = 0;
	while (*s)
	{
		found = 0;
		while (*s && *s++ != set)
			found = 1;
		while (*s && *s == set)
			s++;
		if (found)
			parts++;
	}
	return (parts);
}

static	int	mem_alloc_word(char **split, size_t *word_count, size_t word_len)
{
	split[*word_count] = malloc(sizeof(char) * (word_len + 1));
	if (!split[*word_count])
	{
		while (*word_count)
			free(split[(*word_count)--]);
		free(split[0]);
		free(split);
		return (0);
	}
	(*word_count)++;
	return (1);
}

static	char	**mem_alloc_pp(char *str, char c)
{
	char	**split;
	size_t	i;
	size_t	l;
	size_t	w;

	i = 0;
	w = 0;
	split = (char **)malloc(sizeof(char *) * (word_count(str, c) + 1));
	if (!split)
		return (NULL);
	while (str[i])
	{
		l = 0;
		while (str[i] != c && str[i])
		{
			l++;
			i++;
		}
		if (l > 0 && (str[i] == c || str[i] == '\0') && w < word_count(str, c)
			&& !mem_alloc_word(split, &w, l))
			return (0);
		if (str[i] != '\0')
			i++;
	}
	return (split);
}

static	char	**fill_array(char	*str, char c, char	**split)
{
	size_t	i;
	size_t	x;
	size_t	y;

	x = 0;
	i = 0;
	y = 0;
	while (str[i])
	{
		x = 0;
		while (str[i] != c && str[i] != '\0')
			split[y][x++] = str[i++];
		if (x > 0 && y < word_count(str, c))
			split[y++][x] = '\0';
		if (str[i] == '\0' || str[i + 1] == '\0')
			break ;
		i++;
	}
	split[word_count(str, c)] = NULL;
	return (split);
}

char	**ft_split(char const *str, char c)
{
	char	**res;
	char	*str1;

	str1 = (char *)str;
	res = mem_alloc_pp(str1, c);
	if (!res)
		return (0);
	res = fill_array(str1, c, res);
	return (res);
}
