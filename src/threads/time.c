/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:07:50 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/21 20:08:26 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	get_timeout(int cooldown, long long last_release,
	struct timespec *abstime)
{
	struct timeval	now;
	long long		now_ms;
	long long		wait_ms;

	gettimeofday(&now, NULL);
	now_ms = now.tv_sec * 1000 + now.tv_usec / 1000;

	wait_ms = (last_release + cooldown) - now_ms;

	if (wait_ms < 0)
		wait_ms = 0;

	abstime->tv_sec = now.tv_sec + wait_ms / 1000;
	abstime->tv_nsec = (now.tv_usec * 1000)
		+ ((wait_ms % 1000) * 1000000);

	if (abstime->tv_nsec >= 1000000000)
	{
		abstime->tv_sec++;
		abstime->tv_nsec -= 1000000000;
	}
}

void	ft_usleep(int timesleep, t_data *data)
{
	long long	start;

	start = get_current_time();
	while (!burnout_detected(data))
	{
		if (get_current_time() - start >= timesleep)
			break ;
		usleep(500);
	}
}

long long	timestamp(t_data *data)
{
	return (get_current_time() - data->start_time);
}
