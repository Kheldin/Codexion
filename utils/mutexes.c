/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:17:25 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/24 15:01:59 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	destroy_dongles(int nb_dongle, t_dongle *dongles)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < nb_dongle)
	{
		ret += pthread_cond_destroy(dongles[i].cd_cond);
		ret += pthread_mutex_destroy(dongles[i].dongle_mutex);
		i++;
	}
	return (ret);
}

int	destroy_mutexes(t_config *config, t_coder *coders, t_dongle *dongles)
{
	int ret;

	ret = 0;
	ret += pthread_mutex_destroy(config->config_mutex);
	ret += pthread_mutex_destroy(config->mutex_output);
	ret += pthread_mutex_destroy(config->mutex_queue);
	ret += pthread_cond_destroy(config->cond_top_prio);

	ret += pthread_mutex_destroy(coders->last_compiled_mutex);

	ret += pthread_cond_destroy(dongles->cd_cond);
	ret += pthread_mutex_destroy(dongles->on_cd_mutex);
	ret += destroy_dongles(config->nb_coders, dongles);
	return (ret);
}