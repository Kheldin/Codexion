/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 18:43:37 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/12 18:45:35 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	print_lock(char *msg, pthread_mutex_t *output_mutex)
{
	pthread_mutex_lock(output_mutex);
	printf("%s\n", msg);
	pthread_mutex_unlock(output_mutex);
}
