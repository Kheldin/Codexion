/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_managers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 13:38:18 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/12 13:38:18 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdio.h>

void	*dongle_manager_routine(void *data)
{
	t_dongle_manager	*manager;


    // MUTEX SUR ON_CD ? 
	manager = (t_dongle_manager *)data;
	while (1)
	{
		if (manager->dongle->on_cd && get_time()
			- manager->dongle->last_used >= manager->dongle_cooldown)
		{
            // printf("dongle %d on cd\n\n", manager->dongle->id);
			pthread_mutex_lock(manager->dongle->on_cd_mutex);
			manager->dongle->on_cd = 0;
			pthread_mutex_unlock(manager->dongle->on_cd_mutex);
			pthread_cond_broadcast(manager->dongle->cd_cond);
		}
	}
	return (NULL);
}

static t_dongle_manager	*init_dongles_managers(int nb_dongles,
		t_dongle *dongles, int dongle_cooldown)
{
	t_dongle_manager	*managers;
	int					i;

	managers = ft_calloc(nb_dongles + 1, sizeof(t_dongle_manager));
	if (!managers)
		return (NULL);
	i = 0;
	while (i < nb_dongles)
	{
		managers[i].dongle = &dongles[i];
		managers[i].dongle_cooldown = dongle_cooldown;
		i++;
	}
	return (managers);
}

void	dongles_managers(t_dongle *dongles, int nb_dongles, int dongle_cooldown)
{
	int i;
	pthread_t *dongles_managers_threads;
	t_dongle_manager *dongle_manager;

	dongles_managers_threads = ft_calloc(nb_dongles + 1, sizeof(pthread_t));
	dongle_manager = init_dongles_managers(nb_dongles, dongles,
			dongle_cooldown);
	if (!dongles_managers_threads || !dongle_manager)
		return ;
	i = 0;
	while (i < nb_dongles)
	{
		if (pthread_create(&dongles_managers_threads[i], NULL,
				dongle_manager_routine, &dongle_manager[i]) != 0)
			return ;
		i++;
	}
}