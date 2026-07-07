/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:16:08 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/07 16:16:25 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads(t_data *data, t_coder *coder, pthread_t *monitor, pthread_t *threads)
{
	int			i;

	i = 0;
	while (i < data->nb_coders)
	{
		coder[i].thread_id = i;
		coder[i].data = data;
		coder[i].nb_compiled = 0;
		coder[i].last_compile = get_current_time();
		pthread_create(&threads[i], NULL, routine, &coder[i]);
		i++;
	}
	pthread_create(monitor, NULL, monitor_routine, data);


}

void	join_threads(t_data *data, pthread_t *monitor, pthread_t *threads)
{
	int	i;

	i = -1;
	while (++i < data->nb_coders)
		pthread_join(threads[i], NULL);
	pthread_join(*monitor, NULL);
}

void	creation_threads(t_data *data)
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
	pthread_mutex_init(&data->mutex_burn, NULL);
	data->coder = coder;
	data->burnout_detected = 0;
	create_threads(data, coder, &monitor, threads);
	join_threads(data, &monitor, threads);
	free(coder);
	free(threads);
}
