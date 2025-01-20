/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/03 19:04:32 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/28 17:08:44 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	x;
	size_t			y;

	x = c;
	y = 0;
	while (y < len)
	{
		((unsigned char *)b)[y] = x;
		y++;
	}
	return (b);
}
