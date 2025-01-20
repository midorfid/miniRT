/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putendl_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 17:01:27 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 18:43:57 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	size_t	str_len;

	str_len = ft_strlen(s);
	ft_ignore_result(write(fd, s, str_len));
	ft_putchar_fd('\n', fd);
}
