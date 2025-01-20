/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 14:49:06 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/28 17:16:04 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strnstr(const	char *haystack, const	char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	char	*outcome;

	j = 0;
	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	while (haystack[j] != '\0')
	{
		i = 0;
		if (j > len)
			return (NULL);
		while (needle[i] == haystack[j + i] && j + i < len)
		{
			outcome = (char *)(haystack + j);
			if (ft_strlen(needle) - 1 == i)
				return (outcome);
			if ((j + ft_strlen(needle)) > len)
				return (NULL);
			i++;
		}
		j++;
	}
	return (NULL);
}
