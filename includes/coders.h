/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:42:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/24 14:08:17 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>

typedef struct s_coders {
	int	left;
	int	right;
	int	id;
	pthread_t	thread;
} t_coders;

typedef struct s_dongle {
	float	cooldown;
	char	*location;
	int	id;

} t_dongle;