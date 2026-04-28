/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/28 17:04:26 by kacherch         ###   ########.fr       */
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

static void	enqueue_fifo(t_node **lst, t_node *new)
{
	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}

static void	enqueue_edf(t_node **lst, t_node *new)
{
	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}

void	enqueue(t_node **lst, t_node *new)
{
	if (!(*lst))
		enqueue_fifo(lst, new);
	else if ((*lst)->coder->config->queue_mode == 1)
		enqueue_fifo(lst, new);
	else
		enqueue_edf(lst, new);
	// enqueue_fifo(lst, new);
}

int	is_top_prio(t_coder *coder)
{
	t_coder	*top_coder;
	t_node	*tmp;

	top_coder = coder->config->queue->coder;
	if (top_coder->id == coder->id)
		return (1);
	tmp = coder->config->queue->next;
	while (tmp && tmp->coder->id != coder->id)
	{
		if (tmp->coder->right_dongle == coder->left_dongle)
			return (0);
		if (tmp->coder->left_dongle == coder->right_dongle)
			return (0);
		if (top_coder->right_dongle == coder->left_dongle
			|| top_coder->left_dongle == coder->right_dongle)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
void	dequeue(t_node **queue)
{
	t_node	*node;

	node = *queue;
	if (!*queue)
		return ;
	node->coder->config->queue = node->next;
	free(node);
}

void	init_queue(t_coder *coders, t_config *config)
{
	int i;
	t_node *head;

	i = 0;
	while (i < config->nb_coders)
	{
		if (i == 0)
			head = ft_new_coder_node(&coders[i]);
		else
			enqueue(&head, ft_new_coder_node(&coders[i]));
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