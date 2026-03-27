/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/27 16:47:37 by kacherch         ###   ########.fr       */
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
pthread_t	*create_coders(int nb_coders)
{
	pthread_t	*coders;
	int			*id;
	int			i;

	i = 0;
	coders = ft_calloc((nb_coders + 1), sizeof(pthread_t));
	id = ft_calloc((nb_coders + 1), sizeof(int));
	if (!coders || !id)
		return (NULL);
	while (i < nb_coders)
	{
		id[i] = i;
		if (pthread_create(&coders[i], NULL, init_coders, &id[i]) != 0)
			return (NULL);
		i++;
	}
	nb_coders--;
	while (nb_coders)
		pthread_join(coders[nb_coders--], NULL);
	pthread_join(coders[0], NULL);
	free(id);
	return (coders);
}

t_config	*init_config(char **av)
{
	(void)av;
	return (NULL);
}

int	main(int ac, char **av)
{
	pthread_t	*coders;
	t_config	*config;

	if (ac != 9)
		return (print_instructions());
	coders = create_coders(8);
	if (!coders)
		return (0);
	config = init_config(av);
	free(coders);
	return (0);
}
