/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:09:00 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 09:01:46 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

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

void	enqueue(t_node **lst, t_node *new)
{
	if (!(*lst))
		enqueue_fifo(lst, new);
	else if ((*lst)->coder->config->queue_mode == 1)
		enqueue_fifo(lst, new);
	else
		enqueue_edf(lst, new);
}

int	is_top_prio(t_coder *coder)
{
	t_coder	*top_coder;
	t_node	*tmp;

	if (!coder->config->queue)
		return (1);
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
	t_node	*to_free;

	if (!queue || !*queue)
		return ;
	to_free = *queue;
	*queue = (*queue)->next;
	free(to_free);
}
