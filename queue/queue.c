/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/22 14:54:41 by kacherch         ###   ########.fr       */
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

static t_node	*ft_lstlast(t_node *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_node **lst, t_node *new)
{
	pthread_mutex_lock(new->coder->config->mutex_queue);
	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
	pthread_mutex_unlock(new->coder->config->mutex_queue);
}

int	is_top_prio(t_coder *coder)
{
	int	top_coder;
	t_node	*tmp;

	top_coder = coder->config->queue->coder->id;
	printf("calling %d || top coder id = %d--------\n",coder->id, coder->config->queue->coder->id);
	if (top_coder == coder->id)	
		return (1);
	tmp = coder->config->queue->next;
	while (tmp && tmp->coder->id != coder->id)
	{
		if (tmp->coder->right_dongle == coder->right_dongle || tmp->coder->right_dongle == coder->left_dongle)
			return (0);
		if (tmp->coder->left_dongle == coder->right_dongle || tmp->coder->left_dongle == coder->left_dongle)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
void	dequeue(t_node **queue)
{
	t_node	*node;
	
	pthread_mutex_lock((*queue)->coder->config->mutex_queue);
	node = *queue; 
	if (!queue)
		return ;
	
	// printf("before dequeue in func %d\n", node->coder->config->queue->coder->id);
	node->coder->config->queue = node->next;
	// del(lst->content);
	// printf("after dequeue in func %d\n", node->coder->config->queue->coder->id);
	pthread_mutex_unlock((*queue)->coder->config->mutex_queue);
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
			ft_lstadd_back(&head, ft_new_coder_node(&coders[i]));
		i++;
	}
	config->queue = head;
	// i = 0;
	// while (head)
	// {
	// 	printf("ici %d\n", head->coder->id);
	// 	head = head->next;
	// }
}