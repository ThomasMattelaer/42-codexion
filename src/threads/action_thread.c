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
		&& (timestamp(coder->data)
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
			display_dongle("has taken a dongle", coder, dongle->id);
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
	dongle->last_release = timestamp(coder->data);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (first == second)
		return (0);
	display_dongle("request a dongle", coder, first->id);
	if (!request_single_dongle(coder, first))
		return (0);
	display_dongle("request a dongle", coder, second->id);
	if (!request_single_dongle(coder, second))
	{
		release_single_dongle(coder, first);
		return (0);
	}
	return (1);
}

int	release_both_dongles(t_coder *coder)
{
	release_single_dongle(coder, coder->left_dongle);
	display_dongle("release a dongle", coder, coder->left_dongle->id);
	release_single_dongle(coder, coder->right_dongle);
	display_dongle("release a dongle", coder, coder->right_dongle->id);
	return (1);
}
