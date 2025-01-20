/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_toupper.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 17:09:16 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 18:55:32 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_toupper(int c)
{
	unsigned char	chr;

	chr = c;
	if (chr >= 'a' && chr <= 'z')
		return (chr - 'a' + 'A');
	return (c);
}
