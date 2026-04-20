/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/20 16:45:31 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	ft_queuesize(t_node *queue)
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
t_node	*ft_new_coder_node(t_coder *coder)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->coder = coder;
	node->next = NULL;
	return (node);
}

void	queue_push_front(t_node **queue, t_node *new)
{
	new->next = *queue;
	*queue = new;
}

int	is_top_prio(t_coder *coder, t_node **queue)
{
	pthread_mutex_lock(coder->mutex_queue);
	if ((*queue)->coder == coder)
	{
		pthread_mutex_unlock(coder->mutex_queue);	
		return (1);
	}
	pthread_mutex_unlock(coder->mutex_queue);
	return (0);
}
void	dequeue(t_node **queue)
{
	t_node	*tmp;

	tmp = (*queue)->next;
	if (!queue)
		return ;
	// del(lst->content);
	queue = &tmp;
}

void	init_queue(t_coder *coders, int nb_coders)
{
	int	i;
	t_node	*head;

	i = 0;
	while (i < nb_coders)
	{
		if (i == 0)
		{
			head = ft_new_coder_node(&coders[i]);
		}
		else
		{
			queue_push_front(&head, ft_new_coder_node(&coders[i]));
		}
		coders[i].queue = &head;
		i++;
	}
}