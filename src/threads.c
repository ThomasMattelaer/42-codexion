/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-29 12:33:36 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-29 12:33:36 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void	*arg)
{
	t_data	*data;
	int		i;
	int		over;
	int		j;

	data = (t_data *)arg;
	over = 0;
	usleep(5000);
	while (data->burnout_detected == 0 && over == 0)
	{
		i = -1;
		over = 1;
		while (++i < data->nb_coders)
		{
			if (data->burnout < get_current_time() - data->coder[i].last_compile)
			{
				pthread_mutex_lock(&data->mutex_burn);
				data->burnout_detected = 1;
				pthread_mutex_unlock(&data->mutex_burn);
				j = -1;
				while (++j < data->nb_coders)
					pthread_cond_broadcast(&data->dongles[j].cond);
				display_state("burned out", &data->coder[i], data->burnout, 0);
				return (data); 


			}
			if (data->coder[i].nb_compiled < data->required)
				over = 0;
		}
		usleep(2000);
	}
	return (data);
}

void	*routine(void *arg)
{
	t_coder			*coder;
	int				i;
	int				required;

	coder = (t_coder *)arg;
	required = coder->data->required;
	i = -1;
	while (++i < required && coder->data->burnout_detected == 0)
	{
		if(request_dongles(coder))
			break;
		coder->last_compile = get_current_time();
		display_state("is compiling", coder, coder->data->compile, 0);
		release_dongles(coder);
		if (coder->data->burnout_detected)
			break;
		display_state("is debugging", coder, coder->data->debug, 0);
		if (coder->data->burnout_detected)
			break;
		display_state("is refactoring", coder, coder->data->refactor, 0);
		coder->nb_compiled++;
	}
	return (coder);
}

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
