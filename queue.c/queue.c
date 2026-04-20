/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/18 13:51:13 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	ft_queuesize(t_queue *queue)
{
	int	count;

	count = 0;
	while (queue)
	{
		count++;
		queue = queue->next;
	}
	return (count);
}
t_queue	*ft_new_coder_node(void *data)
{
	t_queue	*coder;

	coder = ft_calloc(1, sizeof(t_queue));
	if (!coder)
		return (NULL);
	coder->data = data;
	coder->next = NULL;
	return (coder);
}

void	enqueue(t_queue **lst, t_queue *new)
{
	new->next = *lst;
	*lst = new;
}

int	is_top_prio(t_coder *coder, t_queue **queue)
{
	if (*queue == coder)
		return (1);
	return (0);
}
void	dequeue(t_queue **lst)
{
	t_queue	*tmp;

	tmp = (*lst)->next;
	if (!lst)
		return ;
	// del(lst->content);
	free(lst);
	lst = &tmp;
}