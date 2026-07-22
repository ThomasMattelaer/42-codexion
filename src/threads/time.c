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

int	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	get_timeout(int cooldown, int last_release, struct timespec *abstime)
{
	struct timeval	now;
	int				remaining;

	remaining = cooldown
		- (get_current_time() - last_release);
	if (remaining < 0)
		remaining = 0;
	gettimeofday(&now, NULL);
	abstime->tv_sec = now.tv_sec + remaining / 1000;
	abstime->tv_nsec = now.tv_usec * 1000 + (remaining % 1000) * 1000000;
	if (abstime->tv_nsec >= 1000000000)
	{
		abstime->tv_sec += 1;
		abstime->tv_nsec -= 1000000000;
	}
}
