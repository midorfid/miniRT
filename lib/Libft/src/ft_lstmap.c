/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 20:29:47 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/28 13:05:58 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*alloc_fail_clr(void *temp, t_list *res, void (*del)(void *))
{
	del (temp);
	ft_lstclear(&res, del);
	return (NULL);
}

void	*alloc_fail_start(void	*temp, void (*del)(void *))
{
	del (temp);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*new_lst;
	void	*temp;

	if (!lst)
		return (NULL);
	temp = f(lst->content);
	new_lst = ft_lstnew(temp);
	if (!new_lst)
		return (alloc_fail_start(temp, del));
	res = new_lst;
	lst = lst->next;
	while (lst)
	{
		temp = f(lst->content);
		new_lst->next = ft_lstnew(temp);
		if (!new_lst->next)
			return (alloc_fail_clr(temp, res, del));
		new_lst = new_lst->next;
		lst = lst->next;
	}
	new_lst->next = NULL;
	return (res);
}
