/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:42:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/29 17:36:21 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>

struct s_config;

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
	long		current_time;
}				t_config;

typedef struct s_coder
{
	t_config	*config;
	long		starting_time;
	int			left;
	int			right;
	int			id;
	int			compiled;
}				t_coder;

void			*ft_calloc(size_t nmemb, size_t size);

int				print_instructions(void);
t_coder			*init_coders(t_config *config);
t_config		*init_config(char **av);
long			get_time(void);