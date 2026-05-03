/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 12:42:41 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/03 12:44:56 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

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

t_data	*init_all(char **av)
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
