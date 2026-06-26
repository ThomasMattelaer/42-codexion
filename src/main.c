/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-24 12:17:28 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-24 12:17:28 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	main(int argc, char **argv)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if(is_valid_data(argc, argv) == 0)
	{
		free(data);
		return (1);
	}
	data->nb_coders = atoi(argv[1]);
	data->burnout = atoi(argv[2]);
	data->compile = atoi(argv[3]);
	data->debug = atoi(argv[4]);
	data->refactor = atoi(argv[5]);
	data->required = atoi(argv[6]);
	data->cooldown = atoi(argv[7]);
	data->scheduler = valid_scheduler(argv[8]);
	routine()
	free(data);
	return (0);
}
