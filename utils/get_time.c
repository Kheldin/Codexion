/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 15:39:36 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/29 16:55:43 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + tv.tv_usec / 1000L);
}

struct timespec	get_interval(void)
{
	struct timespec	time_to_wait;

	time_to_wait.tv_sec = 1;
	time_to_wait.tv_nsec = 1000 * 1000000;
	if (time_to_wait.tv_nsec >= 1000000000)
	{
		time_to_wait.tv_sec /= 1000000000;
		time_to_wait.tv_nsec = time_to_wait.tv_nsec % 1000000000;
	}
	return (time_to_wait);
}
