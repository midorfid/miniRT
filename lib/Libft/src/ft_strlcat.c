/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/07 09:04:52 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/14 14:19:14 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static	size_t	ifs(size_t dstsize, const char *src, char *dst, size_t	dst_len)
{
	if (dstsize == 0)
		return (ft_strlen(src));
	if (dst_len >= dstsize)
		return (dstsize + ft_strlen(src));
	if (dst_len == 0 && dstsize != 0)
		return (ft_strlcpy(dst, src, dstsize));
	return (0);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dst_len;
	size_t	dst_len_res;

	dst_len_res = 0;
	i = 0;
	dst_len = ft_strlen(dst);
	if (ifs(dstsize, src, dst, dst_len))
		return (ifs(dstsize, src, dst, dst_len));
	while (dst[dst_len_res])
		dst_len_res++;
	if (dstsize - dst_len - 1 != 0)
	{
		while (dst_len < dstsize - 1 && src[i])
		{
			dst[dst_len] = src[i];
			dst_len++;
			i++;
		}
	}
	dst[dst_len] = '\0';
	return (dst_len_res + ft_strlen(src));
}
