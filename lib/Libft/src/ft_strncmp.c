/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strncmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 18:19:58 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/28 17:03:51 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strncmp(const	char	*s1, const	char	*s2, size_t x)
{
	size_t	i;

	i = 0;
	if ((s1[i] == '\0' && s2[i] == '\0') || x == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && i < x)
	{
		i++;
	}
	if (i == x)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
