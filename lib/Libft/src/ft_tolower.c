/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_tolower.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 17:27:52 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 18:55:27 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_tolower(int c)
{
	unsigned char	chr;

	chr = c;
	if (chr >= 'A' && chr <= 'Z')
		return (chr - 'A' + 'a');
	return (c);
}
