/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 13:30:32 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 18:54:07 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			i;
	char			*str;
	char			brk_pnt;

	i = 0;
	str = NULL;
	brk_pnt = c;
	if ((s[i] == '\0' && brk_pnt != '\0'))
		return (NULL);
	while (s[i])
	{
		if (s[i] == brk_pnt)
			str = (char *)s + i;
		i++;
	}
	if (s[i] == brk_pnt)
		return ((char *)s + i);
	return (str);
}
