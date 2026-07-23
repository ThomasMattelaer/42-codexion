/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 08:33:42 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 08:33:42 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (!coder->data->burnout_detected
		&& coder->nb_compiled < coder->data->required_compiles)
	{
		if (!take_both_dongles(coder))
			break ;
		if (!compiling(coder))
		{
			release_both_dongles(coder);
			break ;
		}
		release_both_dongles(coder);
		if (!debugging_and_refactoring(coder))
			break ;
	}
	return (coder);
}

void	*monitor_routine(void	*arg)
{
	t_data	*data;
	int		i;
	int		finished_count;

	data = (t_data *) arg;
	i = 0;
	while(1)
	{
		i = data->nb_coders;
		finished_count = 0;
		while (--i >= 0)
		{
			if (is_coder_burned(data, i))
				return (NULL);
			pthread_mutex_lock(&data->coders[i].coder_mutex);
			if (data->coders[i].nb_compiled >= data->required_compiles)
				finished_count++;
			pthread_mutex_unlock(&data->coders[i].coder_mutex);
		}
		if (finished_count == data->nb_coders)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}


