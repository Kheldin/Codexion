/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_everything.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:47:36 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 13:27:37 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	free_threads(pthread_t *coders_threads)
{
	free(coders_threads);
}

void	free_structs(t_dongle *dongles, t_coder *coders)
{
	free(dongles);
	free(coders);
}
