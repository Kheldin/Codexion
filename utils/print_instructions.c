/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_instructions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacherch <kacherch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:39:15 by kacherch          #+#    #+#             */
/*   Updated: 2026/04/27 15:08:43 by kacherch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	print_instructions(void)
{
	printf("Enter the args in the following order please.\n");
	printf("\t=> number_of_coders\n");
	printf("\t=> time_to_burnout\n");
	printf("\t=> time_to_compile\n");
	printf("\t=> time_to_debug\n");
	printf("\t=> time_to_refactor\n");
	printf("\t=> number_of_compiles_required\n");
	printf("\t=> dongle_cooldown\n");
	printf("\t=> scheduler\n");
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
