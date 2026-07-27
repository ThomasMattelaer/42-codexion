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
	if (dongle->is_taken)
		return (0);
	if (dongle->queue->size == 0 || dongle->queue->tab[0].coder_id != coder->id)
		return (0);
	if (dongle->last_release != -1)
	{
		if (timestamp(coder->data) - dongle->last_release < coder->data->cooldown)
			return (0);
	}
	return (1);
}


int	dongle_has_been_taken(t_dongle *first, t_dongle *second, t_coder *coder)
{
	first->is_taken = 1;
	second->is_taken = 1;
	pop_node(first->queue);
	pop_node(second->queue);
	display_dongle("has taken a dongle", coder, first->id);
	display_dongle("has taken a dongle", coder, second->id);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (1);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle		*first;
	t_dongle		*second;
	struct timespec	abstime;

	determine_order(coder, &first, &second);
	if (first == second)
		return (0);
	push_both_dongles(first, second, coder);
	while (1)
	{
		if (burnout_detected(coder->data))
			return (remove_both_nodes(coder, first, second));
		pthread_mutex_lock(&first->mutex);
		pthread_mutex_lock(&second->mutex);
		if (is_dongle_ready(coder, first) && is_dongle_ready(coder, second))
			return (dongle_has_been_taken(first, second, coder));
		pthread_mutex_unlock(&second->mutex);
		get_timeout(coder->data->cooldown, first->last_release, &abstime);
		pthread_cond_timedwait(&first->cond, &first->mutex, &abstime);
		pthread_mutex_unlock(&first->mutex);
	}
}

int	release_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	determine_order(coder, &first, &second);
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	first->is_taken = 0;
	second->is_taken = 0;
	first->last_release = timestamp(coder->data);
	second->last_release = timestamp(coder->data);
	if (!burnout_detected(coder->data))
	{
		display_dongle("release a dongle", coder, first->id);
		display_dongle("release a dongle", coder, second->id);
	}
	pthread_cond_broadcast(&first->cond);
	pthread_cond_broadcast(&second->cond);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (1);
}
