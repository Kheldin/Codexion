/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/29 18:04:32 by kacherch         ###   ########.fr       */
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
	long	start;
	long	end;

	coder = (t_coder *)data;
	start = get_time();
	while (coder->compiled < coder->config->nb_compile_required)
	{
		printf("%d has taken a dongle\n", coder->id);
		printf("%d is compiling\n", coder->id);
		usleep(coder->config->time_to_compile * 1000);
		printf("%d is debugging\n", coder->id);
		usleep(coder->config->time_to_debug * 1000);
		printf("%d is refactoring\n", coder->id);
		usleep(coder->config->time_to_refactor * 1000);
		usleep(coder->config->time_to_refactor * 1000);
		coder->compiled++;
	}
	end = get_time();
	return (NULL);
}

// Create nb_coders thread
pthread_t	*create_coders(t_config *config)
{
	pthread_t	*coders_threads;
	t_coder		*coders;
	int			i;

	i = 0;
	coders_threads = ft_calloc((config->nb_coders + 1), sizeof(pthread_t));
	coders = init_coders(config);
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
	free(coders);
	return (coders_threads);
}

int	main(int ac, char **av)
{
	pthread_t	*coders_threads;
	t_config	*config;

	if (ac != 9)
		return (print_instructions());
	config = init_config(av);
	coders_threads = create_coders(config);
	if (!coders_threads)
		return (0);
	free(coders_threads);
	return (0);
}
