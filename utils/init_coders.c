/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 09:27:18 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/04 13:21:34 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static void	increment_total_comp(t_coder *coder)
{
	pthread_mutex_lock(coder->config->mutex_total_comp);
	coder->config->total_compilations += 1;
	pthread_mutex_lock(coder->last_compiled_mutex);
	coder->nb_compile += 1;
	pthread_mutex_unlock(coder->last_compiled_mutex);
	pthread_mutex_unlock(coder->config->mutex_total_comp);
}

static void	set_coders_dongles(t_coder *coder, t_dongle *dongles, int pos,
		int nb_coders)
{
	if (pos == 0)
	{
		coder->left_dongle = &dongles[nb_coders - 1];
		coder->right_dongle = &dongles[pos];
		return ;
	}
	else if (pos == nb_coders - 1)
	{
		coder->left_dongle = &dongles[pos - 1];
		coder->right_dongle = &dongles[pos];
		return ;
	}
	else
	{
		coder->left_dongle = &dongles[pos - 1];
		coder->right_dongle = &dongles[pos];
	}
}

static void	set_last_compiled_mutex(t_coder *coder)
{
	pthread_mutex_t	*mutex;

	mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!mutex)
		return ;
	if (pthread_mutex_init(mutex, NULL) != 0)
		return ;
	coder->last_compiled_mutex = mutex;
}

t_coder	*init_coders(t_config *config, t_dongle *dongles)
{
	int		i;
	t_coder	*coders;

	coders = ft_calloc(config->nb_coders, sizeof(t_coder));
	if (!coders)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].config = config;
		coders[i].nb_compile = 0;
		coders[i].last_compiled = get_time();
		set_coders_dongles(&coders[i], dongles, i, config->nb_coders);
		set_last_compiled_mutex(&coders[i]);
		i++;
	}
	return (coders);
}

void	*coders_routine(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	if (coder->id % 2 == 0)
		usleep(10000);
	while (coder->nb_compile < coder->config->nb_compile_required)
	{
		pthread_mutex_lock(coder->config->config_mutex);
		if (coder->config->exit == 1)
		{
			pthread_mutex_unlock(coder->config->config_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(coder->config->config_mutex);
		if (compile(coder) == 1 || debug(coder) == 1 || refactor(coder) == 1)
			return (NULL);
		increment_total_comp(coder);
	}
	return (NULL);
}
