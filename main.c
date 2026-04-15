/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/15 16:08:30 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void	*coders_routine(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	if (coder->id % 2 == 1)
		usleep(100000);
	while (coder->nb_compile < coder->config->nb_compile_required)
	{
		compile(coder);
		debug(coder);
		refactor(coder);
		coder->nb_compile++;
	}
	return (NULL);
}

pthread_t	*create_coders(t_config *config, t_coder *coders)
{
	pthread_t	*coders_threads;
	int			i;

	i = 0;
	coders_threads = ft_calloc((config->nb_coders + 1), sizeof(pthread_t));
	if (!coders_threads || !coders)
		return (NULL);
	while (i < config->nb_coders)
	{
		if (pthread_create(&coders_threads[i], NULL, coders_routine,
				&coders[i]) != 0)
			return (NULL);
		i++;
	}
	return (coders_threads);
}

int	main(int ac, char **av)
{
	pthread_t	*coders_threads;
	t_config	*config;
	t_dongle	*dongles;
	t_coder		*coders;
	int			i;
	// t_schedule	*schedule;

	if (ac != 9)
		return (print_instructions());
	config = init_config(av);
	if (!config)
		return (0);
	dongles = init_dongles(config);
	coders = init_coders(config, dongles);
	coders_threads = create_coders(config, coders);
	launch_monitor(coders, config);
	i = 0;
	while (i < config->nb_coders)
		pthread_join(coders_threads[i++], NULL);
	if (!coders_threads)
		return (0);
	// schedule = init_scheduler(config, coders_threads, coders);
	free(coders_threads);
	// free(schedule);
	free(dongles);
	free(coders);
	// Dont forget to destroy every cond and mutexes
	return (0);
}
