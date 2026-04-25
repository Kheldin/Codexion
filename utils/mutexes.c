/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:17:25 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/25 09:56:06 by kacherch         ###   ########.fr       */
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

static int	destroy_coders(int nb_coders, t_coder *coders)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < nb_coders)
	{
		ret += pthread_mutex_destroy(coders[i].last_compiled_mutex);
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

	ret += destroy_coders(config->nb_coders, coders);

	ret += pthread_mutex_destroy(dongles->on_cd_mutex);
	ret += destroy_dongles(config->nb_coders, dongles);
	return (ret);
}
