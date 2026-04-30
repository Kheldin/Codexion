/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 13:30:59 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

static void	increment_total_comp(t_coder *coder)
{
	pthread_mutex_lock(coder->config->mutex_total_comp);
	coder->config->total_compilations += 1;
	pthread_mutex_unlock(coder->config->mutex_total_comp);
}

void	*coders_routine(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (coder->nb_compile < coder->config->nb_compile_required)
	{
		pthread_mutex_lock(coder->config->config_mutex);
		if (coder->config->exit == 1)
		{
			pthread_mutex_unlock(coder->config->config_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(coder->config->config_mutex);
		if (compile(coder) == 1)
			return (NULL);
		if (debug(coder) == 1)
			return (NULL);
		if (refactor(coder) == 1)
			return (NULL);
		coder->nb_compile++;
		increment_total_comp(coder);
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

static void	wait_threads(int nb_coders, pthread_t *coders)
{
	int	i;

	i = 0;
	while (i < nb_coders)
		pthread_join(coders[i++], NULL);
}

int	main(int ac, char **av)
{
	pthread_t	*coders_threads;
	t_config	*config;
	t_dongle	*dongles;
	t_coder		*coders;

	if (ac != 9)
		return (print_instructions());
	config = init_config(av);
	if (!config)
		return (EXIT_FAILURE);
	dongles = init_dongles(config);
	coders = init_coders(config, dongles);
	coders_threads = create_coders(config, coders);
	if (!coders_threads)
		return (EXIT_FAILURE);
	if (launch_monitor(coders, config) == -1)
		return (EXIT_FAILURE);
	wait_threads(config->nb_coders, coders_threads);
	if (destroy_free_everything(config, coders, dongles) > 0)
		fprintf(stderr, "An error happend when destroying mutexes and conds");
	free_threads(coders_threads);
	free_structs(dongles, coders);
	return (EXIT_SUCCESS);
}
