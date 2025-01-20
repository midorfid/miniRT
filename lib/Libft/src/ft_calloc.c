/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/08 01:40:15 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/28 11:56:44 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*mem_alloc;

	mem_alloc = malloc(size * count);
	if (!mem_alloc)
		return (NULL);
	ft_bzero (mem_alloc, size * count);
	return (mem_alloc);
}
