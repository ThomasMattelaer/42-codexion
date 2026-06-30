/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-29 15:07:15 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-29 15:07:15 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	min(int a, int b)
{
	if (a > b)
		return (b);
	return (a);
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	get_timeout(int timeout_ms, struct timespec *abstime)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	abstime->tv_sec = now.tv_sec + timeout_ms / 1000;
	abstime->tv_nsec = now.tv_usec * 1000 + (timeout_ms % 1000) * 1000000;
	if (abstime->tv_nsec >= 1000000000)
	{
		abstime->tv_sec += 1;
		abstime->tv_nsec -= 1000000000;
	}
}
