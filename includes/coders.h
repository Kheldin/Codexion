/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:42:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/27 18:32:03 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>

typedef struct s_dongle
{
	float		cooldown;
	char		*location;
	int			id;

}				t_dongle;

typedef struct s_config
{
	int			nb_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			nb_compile_required;
	int			dongle_cooldown;
}				t_config;

typedef struct s_coders
{
	t_config	config;
	int			left;
	int			right;
	int			id;
}				t_coders;

void			*ft_calloc(size_t nmemb, size_t size);

int				print_instructions(void);