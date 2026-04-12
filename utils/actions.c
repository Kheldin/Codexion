/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 16:24:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/12 18:30:27 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static void	lock_dongles(t_coder *coder)
{
	if (coder->right_dongle->id < coder->left_dongle->id)
	{
		pthread_mutex_lock(coder->right_dongle->dongle_mutex);
		pthread_mutex_lock(coder->left_dongle->dongle_mutex);
		return;
	}
	pthread_mutex_lock(coder->left_dongle->dongle_mutex);
	pthread_mutex_lock(coder->right_dongle->dongle_mutex);
	return;
}

static int	acquire_dongles(t_coder *coder)
{
	while (coder->left_dongle->on_cd)
	{
		// printf("\n\n waiting \n\n");
		// usleep((get_time() - coder->left_dongle->last_used ) * 1000);
		pthread_cond_wait(coder->left_dongle->cd_cond, coder->left_dongle->dongle_mutex);
	}
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d has taken a dongle\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	coder->left_dongle->taken = 1;
	while (coder->right_dongle->on_cd)
	{	
		// printf("\n\n waiting \n\n");
		pthread_cond_wait(coder->right_dongle->cd_cond, coder->right_dongle->dongle_mutex);
		// usleep((get_time() - coder->left_dongle->last_used ) * 1000);
	}
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d has taken a dongle\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	coder->right_dongle->taken = 1;
	return (0);
}

int	compile(t_coder *coder)
{	
	lock_dongles(coder);
	acquire_dongles(coder);
	pthread_mutex_lock(coder->output_mutex);
	printf("%ld %d is compiling\n", get_time() - coder->config->begin_timestamp, coder->id);
	pthread_mutex_unlock(coder->output_mutex);
	usleep(coder->config->time_to_compile * 1000);
	coder->left_dongle->taken = 0;
	coder->right_dongle->taken = 0;
	coder->left_dongle->last_used = get_time();
	coder->right_dongle->last_used = get_time();
	coder->left_dongle->on_cd = 1;
	coder->right_dongle->on_cd = 1;
	pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
	pthread_mutex_unlock(coder->right_dongle->dongle_mutex);
	// pthread_cond_broadcast()
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
