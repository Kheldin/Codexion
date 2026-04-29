/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:58:08 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/29 16:53:07 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	check_exit(t_config *config)
{
	pthread_mutex_lock(config->config_mutex);
	if (config->exit == 1)
	{
		pthread_mutex_unlock(config->config_mutex);
		return (1);
	}
	pthread_mutex_unlock(config->config_mutex);
	return (0);
}

void	set_exit(t_config *config)
{
	pthread_mutex_lock(config->config_mutex);
	config->exit = 1;
	pthread_mutex_unlock(config->config_mutex);
}

int	total_comp_reached(t_config *config)
{
	pthread_mutex_lock(config->mutex_total_comp);
	if (config->total_compilations == config->nb_coders
		* config->nb_compile_required)
	{
		pthread_mutex_unlock(config->mutex_total_comp);
		return (1);
	}
	pthread_mutex_unlock(config->mutex_total_comp);
	return (0);
}
