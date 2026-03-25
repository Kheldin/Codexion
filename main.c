/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/25 17:42:34 by kacherch         ###   ########.fr       */
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

void	create_coders(int nb_coders)
{
	pthread_t	*coders;
	int			*id;
	int			i;

	i = 0;
	coders = ft_calloc((nb_coders + 1), sizeof(pthread_t));
	if (!coders)
		return ;
	id = ft_calloc((nb_coders + 1), sizeof(int *));
	if (!id)
		return ;
	i = 0;
	while (i < nb_coders)
	{
		id[i] = i;
		if (pthread_create(&coders[i], NULL, init_coders, &id[i]) != 0)
			return ;
		i++;
	}
	i = 0;
	while (i < 8)
	{
		pthread_join(coders[i], NULL);
		i++;
	}
	return ;
}

int	main(int ac, char **av)
{
	int	nb_coders;
	int	i;

	(void)ac;
	(void)av;
	i = 0;
	nb_coders = 8;
	create_coders(8);
	// if (!coders)
	// 	return (0); // free everything (coders + inside)
	// while (nb_coders)
	// 	free(coders[--nb_coders]);
	// if (ac != 9)
	// {
	// 	printf("Enter the args in the following order please.\n");
	// 	printf("\t=> number_of_coders\n");
	// 	printf("\t=> time_to_burnout\n");
	// 	printf("\t=> time_to_compile\n");
	// 	printf("\t=> time_to_debug\n");
	// 	printf("\t=> time_to_refactor\n");
	// 	printf("\t=> number_of_compiles_required\n");
	// 	printf("\t=> dongle_cooldown\n");
	// 	printf("\t=> scheduler\n");
	// 	return (0);
	// }
	return (0);
}
/*
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	8

char	*messages[NUM_THREADS];

void	*PrintHello(void *threadid)
{
   long taskid;

   taskid = (long) threadid;
   printf("Thread %ld: %s\n", taskid, messages[taskid]);
   pthread_exit(NULL);
}

int	main(int argc, char *argv[])
{
	pthread_t	threads[NUM_THREADS];
	long		taskids[NUM_THREADS];

	(void)argc;
	(void)argv;
	int res, t;
	messages[0] = "English: Hello World!";
	messages[1] = "French: Bonjour, le monde!";
	messages[2] = "Spanish: Hola al mundo";
	messages[3] = "Klingon: Nuq neH!";
	messages[4] = "German: Guten Tag, Welt!";
	messages[5] = "Russian: Zdravstvuy, mir!";
	messages[6] = "Japan: Sekai e konnichiwa!";
	messages[7] = "Latin: Orbis, te saluto!";
	for(t=0;t<NUM_THREADS;t++)
	{
		taskids[t] = t;
		printf("Creating thread %d\n", t);
		res = pthread_create(&threads[t], NULL, PrintHello,
				(void *) taskids[t]);
		if (res)
		{
			printf("ERROR; return (code from pthread_create() is %d\n", res));
			exit(-1);
		}
	}
	pthread_exit(NULL);
}
*/