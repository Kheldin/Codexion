/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 15:39:36 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/14 14:19:54 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + tv.tv_usec / 1000L);
}
#include <stdio.h>

struct timespec	get_interval()
{
	struct timespec timeToWait;

	timeToWait.tv_sec = 1;
	timeToWait.tv_nsec = 1000 * 1000000;
	if (timeToWait.tv_nsec >= 1000000000)
	{
		timeToWait.tv_sec /= 1000000000;
		timeToWait.tv_nsec = timeToWait.tv_nsec % 1000000000;
	}
	return (timeToWait);
}