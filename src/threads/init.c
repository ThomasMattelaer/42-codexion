/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 07:56:22 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 07:56:22 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*init_dongles(int nb, int mode)
{
	t_dongle	*dongles;
	int			i;

	i = 0;
	dongles = malloc(sizeof(t_dongle) * nb);
	if (!dongles)
		return (NULL);
	while (i < nb)
	{
		dongles[i].id = i;
		dongles[i].is_taken = 0;
		dongles[i].last_release = 0;
		dongles[i].queue = create_heap(nb, mode);
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		i++;
	}
	return (dongles);
}

void	create_threads(t_data *data, t_coder *coder, pthread_t *monitor,
	pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		coder[i].id = i;
		coder[i].data = data;
		coder[i].nb_compiled = 0;
		coder[i].last_compile = get_current_time();
		coder[i].left_dongle = &data->dongles[i];
		coder[i].right_dongle = &data->dongles[(i + 1) % data->nb_coders];
		i++;
	}
	i = 0;
	while(i < data->nb_coders)
	{
		pthread_create(&threads[i], NULL, coder_routine, &coder[i]);
		i++;
	}
	pthread_create(monitor, NULL, monitor_routine, data);
}

void	join_threads(t_data *data, pthread_t *monitor, pthread_t *threads)
{
	int	i;

	i = -1;
	while (++i < data->nb_coders)
	{
		pthread_join(threads[i], NULL);
	}
	pthread_join(*monitor, NULL);
}

void	init_simulation(t_data *data)
{
	pthread_t	*threads;
	pthread_t	monitor;
	t_coder		*coder;

	threads = malloc(sizeof(pthread_t) * data->nb_coders);
	coder = malloc(sizeof(t_coder) * data->nb_coders);
	if (!threads || !coder)
	{
		free(threads);
		free(coder);
		return ;
	}
	pthread_mutex_init(&data->burn_mutex, NULL);
	data->coders = coder;
	create_threads(data, coder, &monitor, threads);
	join_threads(data, &monitor, threads);
	free(coder);
	free(threads);
}

void	init_data(t_data *data, char **argv)
{
	data->nb_coders = atoi(argv[1]);
	data->burnout_time = atoi(argv[2]);
	data->compile = atoi(argv[3]);
	data->debug = atoi(argv[4]);
	data->refactor = atoi(argv[5]);
	data->required_compiles = atoi(argv[6]);
	data->cooldown = atoi(argv[7]);
	data->mode = valid_scheduler(argv[8]);
	data->dongles = init_dongles(data->nb_coders, data->mode);
	data->start_time = get_current_time();
	data->burnout_detected = 0;
}
