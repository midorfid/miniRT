/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 18:46:28 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/28 15:41:43 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!dstsize)
		return (ft_strlen(src));
	while (src[i] && i < dstsize)
	{
		dst[i] = src[i];
		i++;
	}
	if (ft_strlen(src) >= dstsize)
		dst[dstsize - 1] = '\0';
	else
		dst[i] = '\0';
	return (ft_strlen(src));
}
