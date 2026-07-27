/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 07:16:43 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 07:16:43 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	pop_node(t_heap *heap)
{
	t_request	value;
	t_request	empty;

	if (heap->size == 0)
	{
		empty.coder_id = -1;
		return (empty);
	}
	value = heap->tab[0];
	heap->tab[0] = heap->tab[heap->size - 1];
	heap->size--;
	if (heap->size > 0)
		rearrange_heap(heap, 0);
	return (value);
}

void	push_node(t_heap *heap, int coder_id, int deadline)
{
	heap->tab[heap->size].coder_id = coder_id;
	heap->tab[heap->size].deadline = deadline;
	heap->tab[heap->size].arrival_time = get_current_time();
	heap->size++;
	organise_heap(heap, heap->size - 1);
}

void	remove_node(t_heap *heap, int coder_id)
{
	int			index;

	index = 0;
	while (index < heap->size)
	{
		if (heap->tab[index].coder_id == coder_id)
			break ;
		index++;
	}
	if (heap->size == 0 || index == heap->size)
		return ;
	heap->tab[index] = heap->tab[heap->size - 1];
	heap->size--;
	if (heap->size > 0)
	{
		organise_heap(heap, index);
		rearrange_heap(heap, index);
	}
	return ;
}

int	remove_both_nodes(t_coder *coder, t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	remove_node(first->queue, coder->id);
	remove_node(second->queue, coder->id);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (0);
}

void	push_both_dongles(t_dongle *first, t_dongle *second, t_coder *coder)
{
	determine_order(coder, &first, &second);
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	push_node(first->queue, coder->id,
		coder->last_compile + coder->data->burnout_time);
	push_node(second->queue, coder->id,
		coder->last_compile + coder->data->burnout_time);
	display_dongle("request a dongle", coder, first->id);
	display_dongle("request a dongle", coder, second->id);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}
