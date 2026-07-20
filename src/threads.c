/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:33:36 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/07 16:58:34 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void	*arg)
{
	t_data	*data;
	int		i;
	int		completed_coders;
	int		j;

	data = (t_data *)arg;
	usleep(5000);
	while (data->burnout_detected == 0)
	{
		i = data->nb_coders;
		completed_coders = 0;
		while (--i > 0)
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
			if (data->coder[i].nb_compiled >= data->required)
				completed_coders++;
		}
		if (completed_coders == data->nb_coders)
			break;
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
		if(coder->data->burnout_detected)
			break;
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
