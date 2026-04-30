/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:10:55 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 09:28:14 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static void	set_dongle_mutex(t_dongle *dongle)
{
	pthread_mutex_t	*dongle_mutex;
	pthread_mutex_t	*on_cd_mutex;

	on_cd_mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	dongle_mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!dongle_mutex || !on_cd_mutex)
		return ;
	pthread_mutex_init(dongle_mutex, NULL);
	pthread_mutex_init(on_cd_mutex, NULL);
	dongle->on_cd_mutex = on_cd_mutex;
	dongle->dongle_mutex = dongle_mutex;
}

static void	set_dongle_cond(t_dongle *dongle)
{
	pthread_cond_t	*cd_cond;

	cd_cond = ft_calloc(1, sizeof(pthread_cond_t));
	if (!cd_cond)
		return ;
	pthread_cond_init(cd_cond, NULL);
	dongle->cd_cond = cd_cond;
}

t_dongle	*init_dongles(t_config *config)
{
	t_dongle	*dongles;
	int			i;

	dongles = ft_calloc(config->nb_coders + 1, sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		dongles[i].id = i;
		dongles[i].taken = 0;
		dongles[i].available_at = get_time();
		set_dongle_mutex(&dongles[i]);
		set_dongle_cond(&dongles[i]);
		i++;
	}
	return (dongles);
}
