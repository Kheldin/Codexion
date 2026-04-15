/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:42:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/15 16:05:12 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

typedef struct s_dongle
{
	int				id;
	int				taken;
	long			available_at;
	pthread_mutex_t	*dongle_mutex;
	pthread_mutex_t	*on_cd_mutex;
	pthread_cond_t	*cd_cond;
}					t_dongle;

typedef struct s_config
{
	int				nb_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				nb_compile_required;
	int				dongle_cooldown;
	long			begin_timestamp;
}					t_config;

typedef struct s_coder
{
	t_config		*config;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	int				id;
	long			last_compiled;
	int				nb_compile;
	pthread_mutex_t	*output_mutex;
}					t_coder;

typedef struct s_scheduler
{
	t_config		*config;
	t_coder			*coders;
	pthread_t		*coders_threads;
	char			*mode;
}					t_schedule;

typedef struct s_monitor
{
	t_config	*config;
	t_coder		*coders;
} t_monitor;

void				*ft_calloc(size_t nmemb, size_t size);

int					print_instructions(void);

t_coder				*init_coders(t_config *config, t_dongle *dongles);
t_config			*init_config(char **av);
t_dongle			*init_dongles(t_config *config);
t_schedule			*init_scheduler(t_config *config, pthread_t *coders_thread,
						t_coder *coders);

long				get_time(void);

int	compile(t_coder *coder);
int	debug(t_coder *coder);
int	refactor(t_coder *coder);

void	print_lock(char *msg, pthread_mutex_t *output_mutex);
struct timespec	get_interval(void);
void	launch_monitor(t_coder *coders, t_config *config);
