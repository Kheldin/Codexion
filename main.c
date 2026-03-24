/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/24 15:33:34 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "coders.h"
#include <stdlib.h>
#include <pthread.h>

void	*init_coders(void *arg)
{
	printf("arg = %d\n", *(int *)arg);
	return NULL;
}

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	pthread_t coder1;
	int a = 8;
	pthread_create(&coder1, NULL, init_coders, &a);
	pthread_join(coder1, NULL);
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
