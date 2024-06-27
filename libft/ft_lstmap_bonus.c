/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:46:37 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/14 11:46:53 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*list;
	t_list	*temp;

	if (!lst || !f || !del)
		return (0);
	list = 0;
	while (lst)
	{
		temp = f(lst->content);
		new_list = ft_lstnew(temp);
		if (!new_list)
		{
			del(temp);
			ft_lstclear(&list, del);
			return (0);
		}
		ft_lstadd_back(&list, new_list);
		lst = lst->next;
	}
	return (list);
}
