/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-23 13:15:22 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-23 13:15:22 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compiling(t_coder *coder)
{
	if (coder->data->burnout_detected)
		return (0);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile = get_current_time();
	pthread_mutex_unlock(&coder->coder_mutex);
	display_state("is compiling", coder);
	ft_usleep(coder->data->compile, coder->data);
	if (burnout_detected(coder->data))
		return (0);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->nb_compiled++;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (1);
}

int	debugging_and_refactoring(t_coder *coder)
{
	if (burnout_detected(coder->data))
		return (0);
	display_state("is debugging", coder);
	ft_usleep(coder->data->debug, coder->data);
	if (burnout_detected(coder->data))
		return (0);
	display_state("is refactoring", coder);
	ft_usleep(coder->data->refactor, coder->data);
	return (1);
}

int	is_coder_burned(t_data *data, int i)
{
	long long	last;
	int			nb_compiled;
	int			j;

	pthread_mutex_lock(&data->coders[i].coder_mutex);
	last = data->coders[i].last_compile;
	nb_compiled = data->coders[i].nb_compiled;
	pthread_mutex_unlock(&data->coders[i].coder_mutex);
	if(nb_compiled >= data->required_compiles)
		return (0);
	if (get_current_time() - last >= data->burnout_time)
	{
		pthread_mutex_lock(&data->burn_mutex);
		data->burnout_detected = 1;
		pthread_mutex_unlock(&data->burn_mutex);
		j = -1;
		while (++j < data->nb_coders)
			pthread_cond_broadcast(&data->dongles[j].cond);
		display_state("burned out", &data->coders[i]);
		return (1);
	}
	return (0);
}
