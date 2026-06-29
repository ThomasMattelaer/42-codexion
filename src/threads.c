/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-29 12:33:36 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-29 12:33:36 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

void	create_threads(int nb, pthread_t *threads,
			t_coder *coder, t_data *data);
void	join_threads(int nb, pthread_t *threads);

void	*routine(void *arg)
{
	t_coder			*coder;
	int				left;
	int				right;

	coder = (t_coder *)arg;
	coder->last_compile = get_current_time();
	//START TO COMPILE (LOCK THE DONGLES, SLEEP, RELEASE THE DONGLES)
	left = coder->thread_id;
	right = (coder->thread_id + 1) % coder->data->nb_coders;
	if (left > right)
	{
		left = (coder->thread_id + 1) % coder->data->nb_coders;
		right = coder->thread_id;
	}
	pthread_mutex_lock(&coder->data->dongles[left].mutex);
	pthread_mutex_lock(&coder->data->dongles[right].mutex);
	//START TO DEBUG (SLEEP)
	//START TO REFACTOR (SLEEP)
	//while (i < coder->data->required || )
	return (coder);
}

void	creation_threads(t_data *data)
{
	pthread_t	*threads;
	t_coder		*coder;
	int			i;

	threads = malloc(sizeof(pthread_t) * data->nb_coders);
	coder = malloc(sizeof(t_coder) * data->nb_coders);
	if (!threads || !coder)
	{
		free(threads);
		free(coder);
		return ;
	}
	i = 0;
	while (i++ < data->nb_coders)
	{
		coder[i].thread_id = i;
		coder[i].data = data;
		pthread_create(&threads[i], NULL, routine, &coder[i]);
	}
	i = 0;
	while (i++ < data->nb_coders)
		pthread_join(threads[i], NULL);
	free(coder);
	free(threads);
}
