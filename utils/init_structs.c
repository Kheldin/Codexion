/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:10:55 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 09:14:09 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdlib.h>

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

static int	parse_mode(char *mode)
{
	if (ft_strncmp(mode, "fifo", 4) == 0)
		return (1);
	else if ((ft_strncmp(mode, "edf", 3) == 0))
		return (2);
	return (-1);
}

static void	set_config_attr(t_config *config, char **av)
{
	pthread_mutex_t	*mutex_queue;
	pthread_mutex_t	*mutex_total_comp;

	mutex_total_comp = ft_calloc(1, sizeof(pthread_mutex_t));
	mutex_queue = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!mutex_queue || !mutex_total_comp)
		return ;
	pthread_mutex_init(mutex_queue, NULL);
	pthread_mutex_init(mutex_total_comp, NULL);
	config->nb_coders = atoi(av[1]);
	config->time_to_burnout = atoi(av[2]);
	config->time_to_compile = atoi(av[3]);
	config->time_to_debug = atoi(av[4]);
	config->time_to_refactor = atoi(av[5]);
	config->nb_compile_required = atoi(av[6]);
	config->dongle_cooldown = atoi(av[7]);
	config->begin_timestamp = get_time();
	config->exit = 0;
	config->mutex_queue = mutex_queue;
	config->queue = NULL;
	config->queue_mode = parse_mode(av[8]);
	config->total_compilations = 0;
	config->mutex_total_comp = mutex_total_comp;
}

t_config	*init_config(char **av)
{
	static t_config	config;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*mutex_output;
	pthread_cond_t	*cond_top_prio;

	mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	mutex_output = ft_calloc(1, sizeof(pthread_mutex_t));
	cond_top_prio = ft_calloc(1, sizeof(pthread_cond_t));
	if (!mutex || !mutex_output || !cond_top_prio)
		return (NULL);
	pthread_mutex_init(mutex, NULL);
	pthread_mutex_init(mutex_output, NULL);
	pthread_cond_init(cond_top_prio, NULL);
	config.config_mutex = mutex;
	config.cond_top_prio = cond_top_prio;
	config.mutex_output = mutex_output;
	set_config_attr(&config, av);
	return (&config);
}

static void	set_dongle_mutex(t_dongle *dongle)
{
	pthread_mutex_t	*dongle_mutex;
	pthread_mutex_t	*on_cd_mutex;

	on_cd_mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	dongle_mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!dongle_mutex || !on_cd_mutex)
		return ;
	pthread_mutex_init(dongle_mutex, NULL);
	pthread_mutex_init(on_cd_mutex, NULL);
	dongle->on_cd_mutex = on_cd_mutex;
	dongle->dongle_mutex = dongle_mutex;
}

static void	set_dongle_cond(t_dongle *dongle)
{
	pthread_cond_t	*cd_cond;

	cd_cond = ft_calloc(1, sizeof(pthread_cond_t));
	if (!cd_cond)
		return ;
	pthread_cond_init(cd_cond, NULL);
	dongle->cd_cond = cd_cond;
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
		dongles[i].available_at = get_time();
		set_dongle_mutex(&dongles[i]);
		set_dongle_cond(&dongles[i]);
		i++;
	}
	return (dongles);
}

// t_schedule	*init_scheduler(t_config *config, pthread_t *coders_thread,
// 		t_coder *coders)
// {
// 	t_schedule	*schedule;

// 	schedule = ft_calloc(1, sizeof(t_schedule));
// 	if (!schedule)
// 		return (NULL);
// 	schedule->config = config;
// 	schedule->coders_threads = coders_thread;
// 	schedule->coders = coders;
// 	return (schedule);
// }
