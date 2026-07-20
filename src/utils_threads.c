/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 15:07:26 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/11 12:58:06 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_dongles(t_coder *coder)
{
	int	left;
	int	right;

	left = min((coder->thread_id + 1) % coder->data->nb_coders,
			coder->thread_id);
	right = max((coder->thread_id + 1) % coder->data->nb_coders,
			coder->thread_id);
	if (request_dongle(&coder->data->dongles[left], coder, left))
		return (1);
	if (request_dongle(&coder->data->dongles[right], coder, right))
	{
		release_dongle(&coder->data->dongles[left]);
		return (1);
	}
	return (0);
}

int	release_dongles(t_coder *coder)
{
	int	left;
	int	right;

	left = min((coder->thread_id + 1) % coder->data->nb_coders,
			coder->thread_id);
	right = max((coder->thread_id + 1) % coder->data->nb_coders,
			coder->thread_id);
	release_dongle(&coder->data->dongles[left]);
	release_dongle(&coder->data->dongles[right]);
	return (0);
}

int	request_dongle(t_dongle *dongle, t_coder *coder, int dongle_nb)
{
	struct timespec	abstime;
	int				remaining;

	pthread_mutex_lock(&dongle->mutex);
	if (!duplicate_heap(dongle->queue, coder->thread_id))
			push_node(dongle->queue, coder->data->burnout + coder->last_compile, coder->thread_id, coder->data->scheduler);
	while (!dongle->available
		|| get_current_time() - dongle->last_release < coder->data->cooldown || dongle->queue->tab[0].coder_id != coder->thread_id)
	{
		remaining = coder->data->cooldown
		- (get_current_time() - dongle->last_release);
		if (remaining < 0)
		remaining = 0;
		get_timeout(remaining, &abstime);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &abstime);
		if (coder->data->burnout_detected)
		{
			clean_heap(dongle->queue, coder->thread_id, coder->data->scheduler);
			pthread_mutex_unlock(&dongle->mutex);
			return (1);
		}
	}
	pop_node(dongle->queue, coder->data->scheduler);
	dongle->available = 0;
	if (coder->data->burnout_detected)
		return (1);
	display_state("has taken a dongle", coder, coder->data->compile, dongle_nb);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

int	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->last_release = get_current_time();
	if (dongle->queue->size > 0)
		pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

void	display_state(char *s, t_coder *coder, int milliseconds, int dongle_nb)
{
	char	*color;
	int		index;
	int		timestamps;

	index = coder->thread_id + 1;
	timestamps = get_current_time() - coder->data->start_time;
	if (strcmp("is compiling", s) == 0)
		color = "38;2;34;197;34";
	else if (strcmp("is debugging", s) == 0)
		color = "94";
	else if (strcmp("has taken a dongle", s) == 0)
	{
		color = "38;2;251;191;36";
		printf("\x1b[90m[%dms]\x1b[0m \x1b[%sm%s\x1b[0m C%d D%d\n",
		timestamps, color, s, index, dongle_nb + 1);
		usleep(milliseconds * 1000);
		return ;
	}
	else if (strcmp("is refactoring", s) == 0)
		color = "95";
	else if (strcmp("burned out", s) == 0)
		color = "91";
	else
		color = 0;
	printf("\x1b[90m[%dms]\x1b[0m \x1b[%sm%s\x1b[0m C%d\n",
		timestamps, color, s, index);
	usleep(milliseconds * 1000);
}
