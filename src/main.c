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

#include "codexion.h"

t_dongle	*init_dongles(int nb);

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
	data->nb_coders = atoi(argv[1]);
	data->burnout = atoi(argv[2]);
	data->compile = atoi(argv[3]);
	data->debug = atoi(argv[4]);
	data->refactor = atoi(argv[5]);
	data->required = atoi(argv[6]);
	data->cooldown = atoi(argv[7]);
	data->scheduler = valid_scheduler(argv[8]);
	data->dongles = init_dongles(data->nb_coders);
	creation_threads(data);
	free(data);
	return (0);
}

t_dongle	*init_dongles(int nb)
{
	t_dongle	*dongles;
	int			i;

	i = 0;
	dongles = malloc(sizeof(t_dongle) * nb);
	if (!dongles)
		return (NULL);
	while (i < nb)
	{
		dongles[i].available = 1;
		dongles[i].last_release = 0;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		i++;
	}
	return (dongles);
}
