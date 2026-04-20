/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/20 18:38:59 by kacherch         ###   ########.fr       */
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

t_node	*queue_push_front(t_node **queue, t_node *new)
{
	new->next = *queue;
	*queue = new;
	queue = &new;
	return (new);
}

int	is_top_prio(t_coder *coder)
{
	int	top_coder;

	pthread_mutex_lock(coder->mutex_queue);
	top_coder = (*(coder->config->queue))->coder->id;
	if (top_coder == coder->id)
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

void	init_queue(t_coder *coders, t_config *config)
{
	int	i;
	t_node	*head;

	i = 0;
	while (i < config->nb_coders)
	{
		if (i == 0)
			head = ft_new_coder_node(&coders[i]);
		else
			head = queue_push_front(&head, ft_new_coder_node(&coders[i]));
		i++;
	}
	config->queue = &head;
	// i = 0;
	// while (i < config->nb_coders)
	// {
	// 	printf("ici %d\n", head->coder->id);
	// 	i++;
	// }
}