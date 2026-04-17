/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/17 16:58:56 by kacherch         ###   ########.fr       */
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

void	ft_coder_add_front(t_queue **lst, t_queue *new)
{
	new->next = *lst;
	*lst = new;
}
