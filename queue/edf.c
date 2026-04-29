/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 16:57:13 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/29 17:00:20 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <coders.h>

static void	acquire_mutex(t_node *new, t_node *current)
{
	if (new->coder->id > current->coder->id)
	{
		pthread_mutex_lock(new->coder->last_compiled_mutex);
		pthread_mutex_lock(current->coder->last_compiled_mutex);
		return ;
	}
	pthread_mutex_lock(current->coder->last_compiled_mutex);
	pthread_mutex_lock(new->coder->last_compiled_mutex);
}

static void	insert_node(t_node *prev, t_node *current, t_node *new)
{
	if (prev)
	{
		prev->next = new;
		new->next = current;
		return ;
	}
	new->next = current;
	pthread_mutex_lock(new->coder->config->config_mutex);
	new->coder->config->queue = new;
	pthread_mutex_unlock(new->coder->config->config_mutex);
}

void	enqueue_edf(t_node **lst, t_node *new)
{
	t_node	*prev;
	t_node	*current;

	prev = NULL;
	current = *lst;
	while (current && current->coder->id != new->coder->id)
	{
		acquire_mutex(new, current);
		if (new->coder->last_compiled <= current->coder->last_compiled)
		{
			insert_node(prev, current, new);
			pthread_mutex_unlock(current->coder->last_compiled_mutex);
			pthread_mutex_unlock(new->coder->last_compiled_mutex);
			return ;
		}
		pthread_mutex_unlock(current->coder->last_compiled_mutex);
		pthread_mutex_unlock(new->coder->last_compiled_mutex);
		prev = current;
		current = current->next;
	}
}
