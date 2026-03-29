/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 15:39:36 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/29 16:29:31 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stdlib.h>

long    get_time()
{
    struct timeval	tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000L) + tv.tv_usec / 1000L);
}
