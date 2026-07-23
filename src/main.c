/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:03:05 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/21 20:05:40 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if (is_valid_data(argc, argv) == 0)
	{
		free(data);
		return (1);
	}
	init_data(data, argv);
	init_simulation(data);
	return (0);
}
