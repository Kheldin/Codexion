/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/27 18:27:26 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	*init_coders(void *data)
{
	printf("I'm thread number: %d\n", *(int *)data);
	return (NULL);
}

// void	free_coders(pthread_t *coders)
// {
// 	int	i;

// 	i = 0;
// 	while (coders[i])
// 		free(&coders[i++]);
// }

// Create nb_coders thread
pthread_t	*create_coders(t_config *config)
{
	pthread_t	*coders;
	int			*id;
	int			i;

	i = 0;
	coders = ft_calloc((config->nb_coders + 1), sizeof(pthread_t));
	id = ft_calloc((config->nb_coders + 1), sizeof(int));
	if (!coders || !id)
		return (NULL);
	while (i < config->nb_coders)
	{
		id[i] = i;
		if (pthread_create(&coders[i], NULL, init_coders, &id[i]) != 0)
			return (NULL);
		i++;
	}
	i = config->nb_coders;
	while (i)
		pthread_join(coders[i--], NULL);
	pthread_join(coders[0], NULL);
	free(id);
	return (coders);
}

t_config	*init_config(char **av)
{
	static t_config config;

	// config = ft_calloc(1, sizeof(t_config));
	// if (!config)
	// 	return (NULL);
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
	pthread_t	*coders;
	t_config	*config;

	if (ac != 9)
		return (print_instructions());
	config = init_config(av);
	coders = create_coders(config);
	if (!coders)
		return (0);
	free(coders);
	return (0);
}
