/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:42:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/25 16:06:49 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>

typedef struct s_coders {
	int	left;
	int	right;
	int	id;
} t_coders;

typedef struct s_dongle {
	float	cooldown;
	char	*location;
	int	id;

} t_dongle;

void	*ft_calloc(size_t nmemb, size_t size);