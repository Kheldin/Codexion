/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refactor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 09:45:23 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 09:46:06 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	refactor(t_coder *coder)
{
	pthread_mutex_lock(coder->config->mutex_output);
	if (check_exit(coder->config))
	{
		pthread_mutex_unlock(coder->config->mutex_output);
		return (1);
	}
	printf("%ld %d is refactoring\n", get_time()
		- coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->config->mutex_output);
	usleep(coder->config->time_to_refactor * 1000);
	pthread_cond_broadcast(coder->config->cond_top_prio);
	return (0);
}
