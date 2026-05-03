/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 09:25:37 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/03 13:05:44 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	parse_mode(char *mode)
{
	if (!(ft_strlen(mode) == 3 || ft_strlen(mode) == 4))
		return (-1);
	if (ft_strlen(mode) == 4 && (ft_strncmp(mode, "fifo", 4) == 0))
		return (1);
	else if (ft_strlen(mode) == 3 && (ft_strncmp(mode, "edf", 3) == 0))
		return (2);
	return (-1);
}

static int	check_negative_value(char **av, t_config *config)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (atoi(av[i]) < 0)
			return (-1);
		i++;
	}
	return (config->queue_mode);
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
	config->queue_mode = check_negative_value(av, config);
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
	if (config.queue_mode == -1)
	{
		print_instructions();
		return (NULL);
	}
	return (&config);
}
