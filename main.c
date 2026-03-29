/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/29 14:00:20 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	*coders_routine(void *data)
{
	t_coder *casted_data = (t_coder*)data;
	printf("I'm thread number: %d\n", casted_data->id);
	return (NULL);
}

t_coder	*init_coders(t_config *config)
{
	int	i;
	t_coder	*coders;

	coders = ft_calloc(config->nb_coders, sizeof(t_coder));
	if (!coders)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].config = config;
		i++;
	}
	return (coders);
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
		if (pthread_create(&coders_threads[i], NULL, coders_routine, &coders[i]) != 0)
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
	return (&config);
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
