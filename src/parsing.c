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

int	is_valid_data(char **argv)
{
	int	i;
	int	j;

	i = 1;
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
