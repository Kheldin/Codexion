/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:15:48 by kacherch          #+#    #+#             */
/*   Updated: 2026/03/29 18:06:00 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>

static void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n)
	{
		*ptr = (unsigned char)c;
		ptr++;
		n--;
	}
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	nb_bytes;

	if (nmemb != 0 && size > INT_MAX / nmemb)
		return (NULL);
	nb_bytes = nmemb * size;
	ptr = malloc(nb_bytes);
	if (!ptr)
		return (0);
	ft_memset(ptr, 0, nb_bytes);
	return (ptr);
}
