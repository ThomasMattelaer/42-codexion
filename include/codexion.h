/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 07:04:54 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 07:04:54 by tmattela         ###   ########.fr       */
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

typedef struct s_data	t_data;

typedef struct s_request
{
	int			coder_id;
	int			deadline;
	long long	arrival_time;
}	t_request;

typedef struct s_heap
{
	t_request	*tab;
	int			size;
	int			mode;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	int				is_taken;
	long long		last_release;
	t_heap			*queue;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	long long		last_compile;
	int				nb_compiled;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*data;
	pthread_mutex_t	coder_mutex;
}	t_coder;

typedef struct s_data
{
	int				nb_coders;
	int				burnout_time;
	int				compile;
	int				debug;
	int				refactor;
	int				required_compiles;
	int				cooldown;
	long long		start_time;
	int				mode;
	int				burnout_detected;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	burn_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_data;

t_heap		*create_heap(int nb_coder, int mode);
int			find_duplicate(const t_heap *heap, int coder_id);
int			is_higher_priority(t_request a, t_request b, int mode);
void		organise_heap(t_heap *heap, int curr);
void		rearrange_heap(t_heap *heap, int curr);
int			is_not_int_max(char *argv);
int			is_enough_arguments(int argc);
int			valid_scheduler(char *s1);
int			is_valid_data(int argc, char **argv);
t_dongle	*init_dongles(int nb, int mode);
long		get_current_time(void);
void		get_timeout(int cooldown, long long last_release,
				struct timespec *abstime);
void		init_data(t_data *data, char **argv);
t_request	pop_node(t_heap *heap);
void		push_node(t_heap *heap, int coder_id, int deadline);
void		remove_node(t_heap *heap, int coder_id);
void		*monitor_routine(void	*arg);
void		*coder_routine(void *arg);
int			is_dongle_ready(t_coder *coder, t_dongle *dongle);
int			max(int a, int b);
int			min(int a, int b);
int			take_both_dongles(t_coder *coder);
int			release_both_dongles(t_coder *coder);
void		display_state(char *s, t_coder *coder);
void		display_dongle(char *s, t_coder *coder, int nb);
void		init_simulation(t_data *data);
void		ft_usleep(int timesleep, t_data *data);
long long	timestamp(t_data *data);
int			compiling(t_coder *coder);
int			debugging_and_refactoring(t_coder *coder);
int			is_coder_burned(t_data *data, int i);
int			burnout_detected(t_data *data);
void		free_simulation(t_data	*data);
void		safe_broadcast(pthread_cond_t *cond, pthread_mutex_t *mutex);
void		destroy_mutex_and_cond(t_data *data);

#endif
