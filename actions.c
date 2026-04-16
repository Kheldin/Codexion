/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 16:24:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/16 14:13:25 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static void	lock_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec target;

	pthread_mutex_lock(dongle->dongle_mutex);
	pthread_mutex_lock(dongle->on_cd_mutex);
	while (get_time() < dongle->available_at)
	{
		target = get_interval();
		pthread_cond_timedwait(dongle->cd_cond, dongle->on_cd_mutex, &target);
	}
	pthread_mutex_unlock(dongle->on_cd_mutex);
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d has taken a dongle\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	dongle->taken = 1;
}

static int	acquire_dongles(t_coder *coder)
{	
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		lock_dongle(coder->right_dongle, coder);
		lock_dongle(coder->left_dongle, coder);
	}
	else
	{
		lock_dongle(coder->left_dongle, coder);
		lock_dongle(coder->right_dongle, coder);
	}
	return (0);
}

int	compile(t_coder *coder)
{	
	acquire_dongles(coder);
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d is compiling\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	usleep(coder->config->time_to_compile * 1000);
	coder->left_dongle->taken = 0;
	coder->right_dongle->taken = 0;
	coder->left_dongle->available_at = get_time() + coder->config->dongle_cooldown;
	coder->right_dongle->available_at = get_time() + coder->config->dongle_cooldown;
	coder->last_compiled = get_time();
	pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
	pthread_mutex_unlock(coder->right_dongle->dongle_mutex);
	return (0);
}

int	debug(t_coder *coder)
{
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d is debugging\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	usleep(coder->config->time_to_debug * 1000);
	return (0);
}

int	refactor(t_coder *coder)
{
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d is refactoring\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	usleep(coder->config->time_to_refactor * 1000);
	return (0);
}
