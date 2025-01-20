/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 17:04:25 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/20 15:35:34 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		ft_ignore_result(write(fd, "-2147483648", 11));
		return ;
	}
	if (n < 0)
	{
		ft_ignore_result(write(fd, "-", 1));
		n = -n;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		c = n % 10 + '0';
		ft_ignore_result(write(fd, &c, 1));
	}
	if (n < 10)
	{
		c = n + '0';
		ft_ignore_result(write(fd, &c, 1));
	}
}
