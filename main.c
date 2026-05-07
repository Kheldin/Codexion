/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:33:54 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/07 23:53:22 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	wait_threads(int nb_coders, pthread_t *coders)
{
	int	i;

	i = 0;
	while (i < nb_coders)
		pthread_join(coders[i++], NULL);
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	free_threads(data->coders_threads);
	free_structs(data->dongles, data->coders);
	free(data);
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
