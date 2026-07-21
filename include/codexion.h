/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 18:38:59 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/21 18:50:21 by tmattela         ###   ########.fr       */
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

typedef struct s_data t_data; 

typedef struct s_request
{
	int			coder_id;
	long long	deadline;
	long long	arrival_time;
}	t_request;

typedef struct s_heap
{
	t_request	*tab;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	int				is_taken;
	long long		last_release;
	t_heap			queue;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	long long	last_compile;
	t_data		*data;
}	t_coder;

typedef struct s_data
{
	int				nb_coders;
	long long		cooldown;
	long long		burnout_time;
	long long		start_time;
	int				mode;
	int				burnout_detected;
	pthread_mutex_t	write_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_data;

#endif
