/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 15:39:36 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/13 16:10:37 by kacherch         ###   ########.fr       */
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
struct timespec	get_interval(t_dongle *dongle)
{
	struct timespec timeToWait;

	timeToWait.tv_sec = dongle->available_at / 1000;
	timeToWait.tv_nsec = dongle->available_at * 1000000;
	if (timeToWait.tv_nsec >= 1000000000)
	{
		timeToWait.tv_sec /= 1000000000;
		timeToWait.tv_nsec = timeToWait.tv_nsec % 1000000000;
	}
	return (timeToWait);
}