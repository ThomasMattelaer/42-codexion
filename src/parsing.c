/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-24 08:36:02 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-24 08:36:02 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"
#include <limits.h>

int is_not_int_max(char *argv);
int	is_enough_arguments(int argc);

int	is_valid_data(int argc, char **argv)
{
	size_t	i;
	size_t	j;

	i = 1;
	if(is_enough_arguments(argc) == 0)
		return (0);
	while(argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
			{
				printf("[ERROR]: Please provide only numbers as arguments\n");
				return (0);
			}
			j++;
		}
		if (is_not_int_max(argv[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	is_enough_arguments(int argc)
{
	if (argc != 9)
	{
		printf("[ERROR USAGE]: ./codexion number_of_coders time_to_burnout time_to_compile time_to_debug");
		printf("time_to_refactor number_of_compiles_required dongle_cooldown scheduler\n");
		return (0);
	}
	return (1);
}

int is_not_int_max(char *argv)
{
	size_t	i;
	int	nb;

	i = 0;
	nb = 0;
	while (argv[i] >= '0' && argv[i] <= '9')
	{
		if (nb > 214748364 || (nb == 214748364 && (argv[i] - '0') > 7))
		{
			printf("[ERROR]: Overflow detected\n");
    		return (0);
		}
		nb = (nb * 10) + (argv[i] - '0');
		i++;
	}
	return (1);
}


