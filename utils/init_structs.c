/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:10:55 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/01 16:12:34 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdlib.h>

static void	set_coders_dongles(t_coder *coder, t_dongle *dongles, int pos,
		int nb_coders)
{
	pthread_cond_t	dongle_waiting;

	pthread_cond_init(&dongle_waiting, NULL);
	coder->dongles_waiting = &dongle_waiting;
	if (pos == 0)
	{
		coder->left_dongle = &dongles[nb_coders - 1];
		coder->right_dongle = &dongles[pos];
		return;
	}
	else if (pos == nb_coders - 1)
	{
		coder->left_dongle = &dongles[pos];
		coder->right_dongle = &dongles[0];
		return;
	}
	coder->left_dongle = &dongles[pos - 1];
	coder->right_dongle = &dongles[pos];
}

t_coder	*init_coders(t_config *config, t_dongle *dongles)
{
	int				i;
	t_coder			*coders;
	pthread_mutex_t	output_mutex;

	pthread_mutex_init(&output_mutex, NULL);
	coders = ft_calloc(config->nb_coders, sizeof(t_coder));
	if (!coders)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].config = config;
		coders[i].nb_compile = 0;
		coders[i].last_compiled = 0;
		coders[i].output_mutex = &output_mutex;
		set_coders_dongles(&coders[i], dongles, i, config->nb_coders);
		i++;
	}
	return (coders);
}

t_config	*init_config(char **av)
{
	static t_config	config;

	config.nb_coders = atoi(av[1]);
	config.time_to_burnout = atoi(av[2]);
	config.time_to_compile = atoi(av[3]);
	config.time_to_debug = atoi(av[4]);
	config.time_to_refactor = atoi(av[5]);
	config.nb_compile_required = atoi(av[6]);
	config.dongle_cooldown = atoi(av[7]);
	config.begin_timestamp = get_time();
	return (&config);
}

static void	set_dongle_mutex(t_dongle *dongle)
{
	pthread_mutex_t	dongle_mutex;

	pthread_mutex_init(&dongle_mutex, NULL);
	dongle->dongle_mutex = &dongle_mutex;
}

t_dongle	*init_dongles(t_config *config)
{
	t_dongle	*dongles;
	int			i;

	dongles = ft_calloc(config->nb_coders + 1, sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		dongles[i].id = i;
		dongles[i].taken = 0;
		set_dongle_mutex(&dongles[i]);
		i++;
	}
	return (dongles);
}

t_schedule	*init_scheduler(t_config *config, pthread_t *coders_thread,
		t_coder *coders)
{
	t_schedule	*schedule;

	schedule = ft_calloc(1, sizeof(t_schedule));
	if (!schedule)
		return (NULL);
	schedule->config = config;
	schedule->coders_threads = coders_thread;
	schedule->coders = coders;
	return (schedule);
}
