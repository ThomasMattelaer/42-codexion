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
