/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/09 15:18:33 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/20 15:38:35 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*temp = NULL;
	size_t	x;

	x = 0;
	if (!s)
		return (NULL);
	if (start > ft_strlen(s) || len == 0)
		return (ft_strdup(""));
	if (len >= ft_strlen(s) - start)
		temp = malloc(ft_strlen(s) - start + 1);
	if (len < ft_strlen(s) - start)
		temp = malloc(len + 1);
	if (temp == NULL)
		return (NULL);
	while (x < len && s[start])
	{
		temp[x] = s[start];
		x++;
		start++;
	}
	temp[x] = '\0';
	return (temp);
}
