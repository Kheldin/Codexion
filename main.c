/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/03 11:20:43 by kacherch         ###   ########.fr       */
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
		if (compile(coder) == 1 || debug(coder) == 1 || refactor(coder) == 1)
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
	{
		fprintf(stderr, "Error: create_coders: calloc failed\n");
		return (NULL);
	}
	while (i < config->nb_coders)
	{
		if (pthread_create(&coders_threads[i], NULL, coders_routine,
				&coders[i]) != 0)
		{
			fprintf(stderr, "Error: create_coders: pthread_create failed\n");
			free(coders_threads);
			return (NULL);
		}
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

static void	free_data(t_data *data)
{
	if (!data)
		return ;
	free_threads(data->coders_threads);
	free_structs(data->dongles, data->coders);
	free(data);
}

static t_data	*alloc_data(void)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		fprintf(stderr, "Error: init_all: data calloc failed\n");
	return (data);
}

static int	init_components(t_data *data, char **av)
{
	data->config = init_config(av);
	if (!data->config)
	{
		fprintf(stderr, "Error: init_all: init_config failed\n");
		return (-1);
	}
	data->dongles = init_dongles(data->config);
	if (!data->dongles)
	{
		fprintf(stderr, "Error: init_all: init_dongles failed\n");
		return (-1);
	}
	data->coders = init_coders(data->config, data->dongles);
	if (!data->coders)
	{
		fprintf(stderr, "Error: init_all: init_coders failed\n");
		return (-1);
	}
	return (0);
}

static t_data	*init_all(char **av)
{
	t_data	*data;

	data = alloc_data();
	if (!data)
		return (NULL);
	if (init_components(data, av) == -1)
	{
		free_data(data);
		return (NULL);
	}
	data->coders_threads = create_coders(data->config, data->coders);
	if (!data->coders_threads)
	{
		free_data(data);
		return (NULL);
	}
	return (data);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 9)
		return (print_instructions());
	data = init_all(av);
	if (!data)
		return (EXIT_FAILURE);
	if (launch_monitor(data->coders, data->config) == -1)
	{
		fprintf(stderr, "Error: launch_monitor failed\n");
		free_data(data);
		return (EXIT_FAILURE);
	}
	wait_threads(data->config->nb_coders, data->coders_threads);
	if (destroy_free_everything(data->config, data->coders, data->dongles) > 0)
		fprintf(stderr, "Error: destroy mutexes/conds failed\n");
	free_data(data);
	return (EXIT_SUCCESS);
}
