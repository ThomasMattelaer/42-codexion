/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-23 13:45:50 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-23 13:45:50 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_simulation(t_data	*data)
{
	int	i;

	i = -1;
	while (++i < data->nb_coders)
	{
		if(data->dongles[i].queue != NULL)
		{
			free(data->dongles[i].queue->tab);
			free(data->dongles[i].queue);
		}
	}
	if(data->coders != NULL)
		free(data->coders);
	if(data->dongles != NULL)
		free(data->dongles);
	if(data != NULL)
		free(data);
}

void	destroy_mutex_and_cond(t_data *data)
{
	int	i;

	i = 0;
	while(i < data->nb_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		pthread_cond_destroy(&data->dongles[i].cond);
		i++;
	}
	i = 0;
	while(i < data->nb_coders)
	{
		pthread_mutex_destroy(&data->coders[i].coder_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->burn_mutex);
	pthread_mutex_destroy(&data->write_mutex);
}
