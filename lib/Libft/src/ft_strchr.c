/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 17:38:24 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 18:48:29 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strchr(const char	*str, int s)
{
	while(*str){
		if((unsigned char)*str == (unsigned char)s)
			return((char *)str);
		str++;
	}
	if(s == '\0')
		return((char *)str);
	return (NULL);
}
