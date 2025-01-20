/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/09 16:55:30 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 16:58:41 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static	size_t	position(char const	*s1, char const	*set, size_t set_i)
{
	size_t	set_chr_check;
	size_t	str_i;

	str_i = 0;
	set_chr_check = 0;
	while (s1[str_i])
	{
		set_i = 0;
		set_chr_check = 0;
		while (set[set_i])
		{
			if (s1[str_i] != set[set_i])
				set_chr_check++;
			set_i++;
		}
		if (set_chr_check == ft_strlen(set) || s1[str_i + 1] == '\0')
			break ;
		str_i++;
	}
	if (s1[str_i] == '\0')
		return (0);
	return (str_i);
}

static	size_t	reverse_pos(char const	*s1, char const	*set, size_t set_i)
{
	size_t	set_chr_check;
	size_t	set_len;
	size_t	str_i;

	str_i = ft_strlen(s1) - 1;
	set_chr_check = 0;
	set_len = ft_strlen(set);
	while (0 < str_i)
	{
		set_i = 0;
		set_chr_check = 0;
		while (set[set_i])
		{
			if (s1[str_i] != set[set_i])
			{
				if (set_chr_check == set_len - 1)
					return (str_i + 1);
				set_chr_check++;
			}
			set_i++;
		}
		str_i--;
	}
	return (0);
}

static char	*loopi_n(size_t	pos, size_t	rev_pos, char const *s1, char *temp)
{
	size_t	i;

	i = 0;
	while (pos < rev_pos)
	{
		temp[i] = s1[pos];
		i++;
		pos++;
	}
	temp[i] = '\0';
	return (temp);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*temp;
	size_t	set_i;
	size_t	pos;
	size_t	rev_pos;

	rev_pos = 0;
	pos = 0;
	set_i = 0;
	if ((s1[0] == '\0' && set[0] == '\0') || s1[0] == '\0')
		return (ft_strdup(""));
	if (set[0] == '\0')
		return (ft_strdup(s1));
	pos = position(s1, set, set_i);
	rev_pos = reverse_pos(s1, set, set_i);
	if ((pos == ft_strlen(s1) - 1 && rev_pos == 0) || (!pos && !rev_pos))
		return (ft_strdup(""));
	temp = malloc(rev_pos - pos + 1);
	if (temp == NULL)
		return (NULL);
	return (loopi_n(pos, rev_pos, s1, temp));
}
