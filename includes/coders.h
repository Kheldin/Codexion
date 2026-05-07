/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:42:49 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/07 18:51:14 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_node	t_node;

typedef struct s_dongle
{
	int					id;
	long				available_at;
	pthread_mutex_t		*dongle_mutex;
	pthread_mutex_t		*on_cd_mutex;
	pthread_cond_t		*cd_cond;
}						t_dongle;

typedef struct s_config
{
	int					nb_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					nb_compile_required;
	int					dongle_cooldown;
	long				begin_timestamp;
	int					exit;
	int					total_compilations;
	int					queue_mode;
	pthread_mutex_t		*config_mutex;
	pthread_mutex_t		*mutex_output;
	pthread_mutex_t		*mutex_queue;
	pthread_mutex_t		*mutex_total_comp;
	pthread_cond_t		*cond_top_prio;
	t_node				*queue;
}						t_config;

typedef struct s_coder
{
	t_config			*config;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	int					id;
	long				last_compiled;
	int					nb_compile;
	pthread_mutex_t		*last_compiled_mutex;
}						t_coder;

typedef struct s_monitor
{
	t_config			*config;
	t_coder				*coders;
}						t_monitor;

typedef struct s_node
{
	t_coder				*coder;
	t_node				*next;
}						t_node;

typedef struct s_data
{
	t_config			*config;
	t_dongle			*dongles;
	t_coder				*coders;
	pthread_t			*coders_threads;
}						t_data;

void					*ft_calloc(size_t nmemb, size_t size);

int						print_instructions(void);
int						ft_isdigit(int c);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_strlen(char *str);

t_coder					*init_coders(t_config *config, t_dongle *dongles);
void					*coders_routine(void *data);
t_config				*init_config(char **av);
t_dongle				*init_dongles(t_config *config);

long					get_time(void);

int						compile(t_coder *coder);
int						debug(t_coder *coder);
int						refactor(t_coder *coder);

void					print_lock(char *msg, pthread_mutex_t *output_mutex);
struct timespec			get_interval(void);
int						launch_monitor(t_coder *coders, t_config *config);

void					set_exit(t_config *config);
int						check_exit(t_config *config);
int						total_comp_reached(t_config *config);

t_node					*ft_new_coder_node(t_coder *coder);
void					enqueue(t_node **lst, t_node *new);
int						is_top_prio(t_coder *coder);
void					dequeue(t_node **lst);

int						destroy_free_everything(t_config *config,
							t_coder *coders, t_dongle *dongles);

void					enqueue_edf(t_node **lst, t_node *new);

void					free_structs(t_dongle *dongles, t_coder *coders);
void					free_threads(pthread_t *coders_threads);

t_data					*init_all(char **av);

void					free_data(t_data *data);
