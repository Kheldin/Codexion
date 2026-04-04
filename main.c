/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/04 18:40:23 by kacherch         ###   ########.fr       */
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
	while (coder->nb_compile < coder->config->nb_compile_required)
	{
		pthread_mutex_lock(coder->left_dongle->dongle_mutex);
		pthread_mutex_lock(coder->right_dongle->dongle_mutex);
		while (coder->left_dongle->taken)
		{
			pthread_cond_wait(coder->dongles_waiting, coder->left_dongle->dongle_mutex);
		}
		coder->left_dongle->taken = 1;
		while (coder->right_dongle->taken)
		{	
			pthread_cond_wait(coder->dongles_waiting, coder->right_dongle->dongle_mutex);
		}
		coder->right_dongle->taken = 1;
		printf("thread nb = %d, adress = %p\n", coder->id, coder->output_mutex);
		pthread_mutex_lock(coder->output_mutex);
		printf("%ld %d has taken a dongle\n", get_time() - coder->config->begin_timestamp, coder->id);
		
		// ========================= Compiling =================================
		pthread_mutex_unlock(coder->output_mutex);
		pthread_mutex_lock(coder->output_mutex);
		printf("%ld %d is compiling\n", get_time() - coder->config->begin_timestamp, coder->id);
		pthread_mutex_unlock(coder->output_mutex);
		usleep(coder->config->time_to_compile * 1000);
		coder->left_dongle->taken = 0;
		coder->right_dongle->taken = 0;
		// pthread_cond_broadcast(coder->dongles_waiting);
		pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
		pthread_mutex_unlock(coder->right_dongle->dongle_mutex);

		pthread_mutex_lock(coder->output_mutex);
		printf("%ld %d is debugging\n", get_time() - coder->config->begin_timestamp, coder->id);
		pthread_mutex_unlock(coder->output_mutex);
		usleep(coder->config->time_to_debug * 1000);
		pthread_mutex_lock(coder->output_mutex);
		printf("%ld %d is refactoring\n", get_time() - coder->config->begin_timestamp, coder->id);
		pthread_mutex_unlock(coder->output_mutex);
		usleep(coder->config->time_to_refactor * 1000);
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
	i = config->nb_coders;
	while (i)
		pthread_join(coders_threads[i--], NULL);
	pthread_join(coders_threads[0], NULL);
	return (coders_threads);
}

int	main(int ac, char **av)
{
	pthread_t	*coders_threads;
	pthread_cond_t	dongles_waiting;
	t_config	*config;
	t_dongle	*dongles;
	t_coder		*coders;
	// t_schedule	*schedule;
	long	begin_timestamp;

	begin_timestamp = get_time();
	if (ac != 9)
		return (print_instructions());
	config = init_config(av);
	if (!config)
		return (0);
	pthread_cond_init(&dongles_waiting, NULL);
	dongles = init_dongles(config);
	coders = init_coders(config, dongles);
	coders_threads = create_coders(config, coders);
	if (!coders_threads)
		return (0);
	// schedule = init_scheduler(config, coders_threads, coders);
	free(coders_threads);
	// free(schedule);
	free(dongles);
	free(coders);
	return (0);
}
