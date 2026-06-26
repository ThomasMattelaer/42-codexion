/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-24 08:36:02 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-24 08:36:02 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *arg)
{
	t_coder	*coder;
	struct timeval tv;
	int	i;

	i = 0;
	coder = (t_coder *)arg;
	gettimeofday(&tv, NULL);
	long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	printf("time %lu ", ms);
	printf("numero de thread: %d", coder->thread_id);
	//while (i < coder->data->required || )
}

void	creation_threads(t_data *data)
{
	pthread_t	*threads;
	t_coder		*coder;
	int			i;
	int			nb;

	i = 0;
	nb = data->nb_coders; 
	threads = malloc(sizeof(pthread_t) * nb);
	coder = malloc(sizeof(t_coder) * nb);
	if(!threads || !coder)
	{
		free(threads);
		free(coder);
		return ;
	}
	create_threads(nb, threads, coder, data);
	join_threads(nb, threads);
	free(coder);
	free(threads);
}
void	create_threads(int nb, pthread_t *threads, t_coder *coder, t_data *data)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		coder[i].thread_id = i;
		coder[i].data = data;
		p_thread_create(&threads[i], NULL, routine, &coder[i]);
		i++;
	}
}
void	join_threads(int nb, pthread_t *threads)
{
	int i;

	i = 0;
	while(i < nb)
	{
		p_thread_join(threads[i], NULL);
		i++;
	}
}
