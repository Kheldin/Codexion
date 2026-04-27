/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:17:25 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/27 11:08:24 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	destroy_dongles(int nb_dongle, t_dongle *dongles)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < nb_dongle)
	{
		ret += pthread_cond_destroy(dongles[i].cd_cond);
		ret += pthread_mutex_destroy(dongles[i].dongle_mutex);
		free(dongles[i].cd_cond);
		free(dongles[i].dongle_mutex);
		free(dongles[i].on_cd_mutex);
		i++;
	}
	return (ret);
}

static int	destroy_coders(int nb_coders, t_coder *coders)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < nb_coders)
	{
		ret += pthread_mutex_destroy(coders[i].last_compiled_mutex);
		free(coders[i].last_compiled_mutex);
		i++;
	}
	return (ret);
}

void	free_queue(t_node **queue)
{
	t_node	*tmp;

	tmp = *queue;
	while (*queue)
	{
		tmp = (*queue)->next;
		free(*queue);
		*queue = tmp;
	}
}

int	destroy_free_everything(t_config *config, t_coder *coders, t_dongle *dongles)
{
	int ret;

	ret = 0;
	if (config->queue)
		free_queue(&config->queue);
	ret += pthread_mutex_destroy(config->config_mutex);
	ret += pthread_mutex_destroy(config->mutex_output);
	ret += pthread_mutex_destroy(config->mutex_queue);
	ret += pthread_cond_destroy(config->cond_top_prio);

	ret += destroy_coders(config->nb_coders, coders);

	ret += pthread_mutex_destroy(dongles->on_cd_mutex);
	ret += destroy_dongles(config->nb_coders, dongles);
	free(config->cond_top_prio);
	free(config->mutex_queue);
	free(config->mutex_output);
	free(config->mutex_total_comp);
	free(config->config_mutex);
	return (ret);
}
