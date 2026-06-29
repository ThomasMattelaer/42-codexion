/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-29 15:07:26 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-29 15:07:26 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



int	request_dongle(t_dongle *dongle, t_coder *coder)
{
    pthread_mutex_lock(&dongle->mutex);
    while (!dongle->available || get_current_time() - dongle->last_release < coder->data->cooldown)
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    dongle->available = 0;
    pthread_mutex_unlock(&dongle->mutex);
    return (0);
}

int	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle-> last_release = get_current_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (0) ;
}

void	display_state(char *s, int index, int milliseconds)
{
	printf("[%d] %d %s\n", get_current_time(),  index, s);
	usleep(milliseconds);
}


