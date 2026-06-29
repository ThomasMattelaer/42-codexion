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

int	request_dongles(t_coder *coder);
int	release_dongles(t_coder *coder);

void	*routine(void *arg)
{
	t_coder			*coder;
	int				i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->data->required)
	{
		request_dongles(coder);
		coder->last_compile = get_current_time();
		display_state("is compiling", coder->thread_id + 1, coder->data->compile);
		printf("test");
		release_dongles(coder);
		// debug();
		// refactor();
		i++;
	}
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
	i = -1;
	while (++i < data->nb_coders)
	{
		coder[i].thread_id = i;
		coder[i].data = data;
		pthread_create(&threads[i], NULL, routine, &coder[i]);
	}
	i = -1;
	while (++i < data->nb_coders)
		pthread_join(threads[i], NULL);
	free(coder);
	free(threads);
}
int	request_dongles(t_coder *coder)
{
	int	left;
	int	right;

	printf("thread %d requesting dongle, available=%d\n", coder->thread_id, coder->data->dongles->available);
	left = min((coder->thread_id + 1) % coder->data->nb_coders, coder->thread_id);
	right = max((coder->thread_id + 1) % coder->data->nb_coders, coder->thread_id);
	request_dongle(&coder->data->dongles[left], coder);
	request_dongle(&coder->data->dongles[right], coder);
	return (0);
}

int	release_dongles(t_coder *coder)
{
	int	left;
	int	right;

	left = min((coder->thread_id + 1) % coder->data->nb_coders, coder->thread_id);
	right = max((coder->thread_id + 1) % coder->data->nb_coders, coder->thread_id);
	release_dongle(&coder->data->dongles[left]);
	release_dongle(&coder->data->dongles[right]);
	return(0);

}
