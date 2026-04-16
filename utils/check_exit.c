/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:58:08 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/16 16:27:46 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int check_exit(t_config *config)
{
    pthread_mutex_lock(config->config_mutex);
    if (config->exit == 1)
    {
        pthread_mutex_unlock(config->config_mutex);
        return (1);
    }
    pthread_mutex_unlock(config->config_mutex);
    return (0);
}

void    set_exit(t_config *config)
{
    pthread_mutex_lock(config->config_mutex);
    config->exit = 1;
    pthread_mutex_unlock(config->config_mutex);
}