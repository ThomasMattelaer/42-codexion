/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 12:17:48 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/11 12:59:00 by tmattela         ###   ########.fr       */
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

typedef struct s_coder t_coder;

typedef struct s_request
{
	int	arrival;
	int	burnout_time;
	int	coder_id;
} t_request;

typedef struct s_heap
{
	int			size;
	t_request	*tab;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	int				last_release;
	t_heap			*queue;
}	t_dongle;

typedef struct s_data
{
	int				nb_coders;
	int				start_time;
	int				burnout;
	int				compile;
	int				debug;
	int				refactor;
	int				required;
	int				cooldown;
	int				scheduler;
	int				burnout_detected;
	pthread_mutex_t	mutex_burn;
	t_dongle		*dongles;
	t_coder			*coder;
}	t_data;

typedef struct s_coder
{
	int		thread_id;
	int		last_compile;
	int		nb_compiled;
	t_data	*data;
}	t_coder;

int			is_valid_data(int argc, char **argv);
int			is_enough_arguments(int argc);
int			valid_scheduler(char *s1);
void		creation_threads(t_data *data);
int			get_current_time(void);
int			min(int a, int b);
int			max(int a, int b);
int			request_dongle(t_dongle *dongle, t_coder *coder, int dongle_nb);
int			release_dongle(t_dongle *dongle);
int			request_dongles(t_coder *coder);
int			release_dongles(t_coder *coder);
void		display_state(char *s, t_coder *coder, int milliseconds, int dongle_nb);
void		get_timeout(int timeout_ms, struct timespec *abstime);
void		*monitor_routine(void	*arg);
void		*routine(void *arg);
void		organise_heap(t_heap *heap, int curr, int mode);
int			duplicate_heap(const t_heap *heap, int coder_id);
t_heap		*create_heap(int nb_coder);
t_request	pop_node(t_heap *heap, int mode);
void		push_node(t_heap *heap, int burnout_time, int coder_id, int mode);
void		clean_heap(t_heap *heap, int coder_id, int mode);
int			is_higher_priority(t_request a, t_request b, int mode);

#endif
