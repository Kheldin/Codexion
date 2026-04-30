/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_everything.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:47:36 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 11:01:39 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	free_threads(pthread_t *coders_threads, pthread_t *monitor_thread)
{
	free(coders_threads);
	free(monitor_thread);
}

void	free_structs(t_monitor *monitor, t_dongle *dongles, t_coder *coders)
{
	free(dongles);
	free(coders);
	free(monitor);
}
