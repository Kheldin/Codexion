/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 09:27:18 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/30 09:28:48 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static void	set_coders_dongles(t_coder *coder, t_dongle *dongles, int pos,
	int nb_coders)
{
	if (pos == 0)
	{
		coder->left_dongle = &dongles[nb_coders - 1];
		coder->right_dongle = &dongles[pos];
		return ;
	}
	else if (pos == nb_coders - 1)
	{
		coder->left_dongle = &dongles[pos - 1];
		coder->right_dongle = &dongles[pos];
		return ;
	}
	else
	{
		coder->left_dongle = &dongles[pos - 1];
		coder->right_dongle = &dongles[pos];
	}
}

static void	set_last_compiled_mutex(t_coder *coder)
{
	pthread_mutex_t	*mutex;

	mutex = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!mutex)
		return ;
	if (pthread_mutex_init(mutex, NULL) != 0)
		return ;
	coder->last_compiled_mutex = mutex;
}

t_coder	*init_coders(t_config *config, t_dongle *dongles)
{
	int		i;
	t_coder	*coders;

	coders = ft_calloc(config->nb_coders, sizeof(t_coder));
	if (!coders)
		return (NULL);
	i = 0;
	while (i < config->nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].config = config;
		coders[i].nb_compile = 0;
		coders[i].last_compiled = get_time();
		set_coders_dongles(&coders[i], dongles, i, config->nb_coders);
		set_last_compiled_mutex(&coders[i]);
		i++;
	}
	return (coders);
}
