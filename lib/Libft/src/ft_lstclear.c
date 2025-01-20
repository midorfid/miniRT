/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstclear.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 20:09:08 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/20 21:02:14 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*no;
	t_list	*leaks;

	no = *lst;
	while (no != NULL)
	{
		leaks = no->next;
		ft_lstdelone(no, del);
		no = leaks;
	}
	*lst = NULL;
}
