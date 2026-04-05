/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 16:24:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/05 18:29:02 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static int	acquire_dongles(t_coder *coder)
{
	pthread_mutex_lock(coder->left_dongle->dongle_mutex);
	pthread_mutex_lock(coder->right_dongle->dongle_mutex);
	while (coder->left_dongle->taken)
	{
		pthread_cond_wait(coder->dongles_waiting, coder->left_dongle->dongle_mutex);
	}
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d has taken a dongle\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	coder->left_dongle->taken = 1;
	while (coder->right_dongle->taken)
	{	
		pthread_cond_wait(coder->dongles_waiting, coder->right_dongle->dongle_mutex);
	}
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d has taken a dongle\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	coder->right_dongle->taken = 1;
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
