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
	struct timespec	abstime;
	int				remaining;

	pthread_mutex_lock(&dongle->mutex);
	display_state("has taken a dongle", coder, coder->data->compile);
	while (!dongle->available
		|| get_current_time() - dongle->last_release < coder->data->cooldown)
	{
		remaining = coder->data->cooldown
			- (get_current_time() - dongle->last_release);
		if (remaining < 0)
			remaining = 0;
		get_timeout(remaining, &abstime);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &abstime);
	}
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

int	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->last_release = get_current_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

void	display_state(char *s, t_coder *coder, int milliseconds)
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
		color = "38;2;251;191;36";
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
