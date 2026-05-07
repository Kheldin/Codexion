/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_instructions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:39:15 by kacherch          #+#    #+#             */
/*   Updated: 2026/05/07 19:48:26 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	print_instructions(void)
{
	fprintf(stderr, "Enter the args in the following order please.\n");
	fprintf(stderr, "scheduler should be fifo or edf.\n");
	fprintf(stderr, "Other value must not be negative.\n");
	fprintf(stderr, "\t=> number_of_coders\n");
	fprintf(stderr, "\t=> time_to_burnout\n");
	fprintf(stderr, "\t=> time_to_compile\n");
	fprintf(stderr, "\t=> time_to_debug\n");
	fprintf(stderr, "\t=> time_to_refactor\n");
	fprintf(stderr, "\t=> number_of_compiles_required\n");
	fprintf(stderr, "\t=> dongle_cooldown\n");
	fprintf(stderr, "\t=> scheduler\n");
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}
