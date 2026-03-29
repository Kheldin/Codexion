/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:10:55 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/29 18:07:15 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdlib.h>

t_coder	*init_coders(t_config *config)
{
	int		i;
	t_coder	*coders;

	coders = ft_calloc(config->nb_coders, sizeof(t_coder));
	if (!coders)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].config = config;
		coders[i].compiled = 0;
		i++;
	}
	return (coders);
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
	config.current_time = 0;
	return (&config);
}

t_dongle	*init_dongles(t_config config)
{
	t_dongle	*dongles;
	int			i;

	dongles = ft_calloc(config.nb_coders + 1, sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < config.nb_coders)
	{
		dongles[i].id = i;
	}
}
