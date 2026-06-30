/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-24 12:17:48 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-24 12:17:48 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	int				last_release;
}	t_dongle;

typedef struct s_data
{
	int			nb_coders;
	int			start_time;
	int			burnout;
	int			compile;
	int			debug;
	int			refactor;
	int			required;
	int			cooldown;
	int			scheduler;
	t_dongle	*dongles;
}	t_data;

typedef struct s_coder
{
	int		thread_id;
	int		last_compile;
	t_data	*data;
}	t_coder;

int		is_valid_data(int argc, char **argv);
int		is_enough_arguments(int argc);
int		valid_scheduler(char *s1);
void	creation_threads(t_data *data);
int		get_current_time(void);
int		request_dongle(t_dongle *dongle, t_coder *coder);
int		min(int a, int b);
int		max(int a, int b);
int		request_dongle(t_dongle *dongle, t_coder *coder);
int		release_dongle(t_dongle *dongle);
void	display_state(char *s, t_coder *coder, int milliseconds);
void	get_timeout(int timeout_ms, struct timespec *abstime);

#endif
