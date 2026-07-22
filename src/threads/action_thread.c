/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 09:14:36 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 09:14:36 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_dongle_ready(t_coder *coder, t_dongle *dongle)
{
	return (!dongle->is_taken
		&& (dongle->queue->size > 0
			&& dongle->queue->tab[0].coder_id == coder->id)
		&& (get_current_time()
			- dongle->last_release >= coder->data->cooldown));
}

int	request_single_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	abstime;

	pthread_mutex_lock(&dongle->mutex);
	push_node(dongle->queue, coder->id,
		coder->last_compile + coder->data->burnout_time);
	while (1)
	{
		if (coder->data->burnout_detected)
		{
			remove_node(dongle->queue, coder->id);
			pthread_cond_broadcast(&dongle->cond);
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		if (is_dongle_ready(coder, dongle))
		{
			dongle->is_taken = 1;
			pop_node(dongle->queue);
			pthread_mutex_unlock(&dongle->mutex);
			return (1);
		}
		get_timeout(coder->data->cooldown, dongle->last_release, &abstime);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &abstime);
	}
}

int	release_single_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_taken = 0;
	dongle->last_release = get_current_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

int	take_both_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
		return (0);
	if (!request_single_dongle(coder, coder->left_dongle))
		return (0);
	if (!request_single_dongle(coder, coder->right_dongle))
	{
		release_single_dongle(coder, coder->left_dongle);
		return (0);
	}
	return (1);
}

int	release_both_dongles(t_coder *coder)
{
	release_single_dongle(coder, coder->left_dongle);
	release_single_dongle(coder, coder->right_dongle);
	return (1);
}
