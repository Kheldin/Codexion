/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:21:10 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/16 14:33:20 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static void	*monitor_routine(void *data)
{
	t_monitor	*monitor;
	int		i;

	monitor = (t_monitor *)data;
	while (1)
	{
		i = 0;
		while (i < monitor->config->nb_coders)
		{
			pthread_mutex_lock(monitor->coders[i].last_compiled_mutex);
			if (get_time() > monitor->coders[i].last_compiled + monitor->config->time_to_burnout)
			{
				pthread_mutex_lock(monitor->coders[i].output_mutex);
				printf("%ld %d burned out\n", get_time() - monitor->config->begin_timestamp, monitor->coders[i].id);
				pthread_mutex_unlock(monitor->coders[i].output_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(monitor->coders[i].last_compiled_mutex);
			i++;
		}
	}
}

static t_monitor	*init_monitor(t_coder *coders, t_config *config)
{
	t_monitor *monitor;

	monitor = ft_calloc(1, sizeof(t_monitor));
	if (!monitor)
		return (NULL);
	monitor->coders = coders;
	monitor->config = config;
	return (monitor);
}

void	launch_monitor(t_coder *coders, t_config *config)
{
	pthread_t *monitor_thread;
	t_monitor	*monitor;

	monitor = init_monitor(coders, config);
	monitor_thread = ft_calloc(1, sizeof(pthread_t));
	if (!monitor_thread || !monitor)
		return ;
	pthread_create(monitor_thread, NULL, monitor_routine, monitor);
}