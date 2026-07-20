/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:59:32 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/11 12:35:36 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_higher_priority(t_request a, t_request b, int mode)
{
	if (mode == 2)
	{
		if (a.burnout_time == b.burnout_time)
			return (a.coder_id < b.coder_id);
		return (a.burnout_time < b.burnout_time);
	}
	return (a.arrival < b.arrival);
}
void	rearrange_heap(t_heap *heap, int curr, int mode)
{
	int			left;
	int			right;
	int			smallest;
	t_request	tmp;

	while (2 * curr + 1 < heap->size)
	{
		left = 2 * curr + 1;
		right = 2 * curr + 2;
		smallest = left;
		if (right < heap->size
			&& is_higher_priority(heap->tab[right], heap->tab[left], mode))
			smallest = right;
		if (is_higher_priority(heap->tab[smallest], heap->tab[curr], mode))
		{
			tmp = heap->tab[smallest];
			heap->tab[smallest] = heap->tab[curr];
			heap->tab[curr] = tmp;
			curr = smallest;
		}
		else
			break ;
	}
}

t_request	pop_node(t_heap *heap, int mode)
{
	t_request	value;

	if (heap->size == 0)
	{
		t_request empty = {0};
		return (empty);
	}
	value = heap->tab[0];
	heap->tab[0] = heap->tab[heap->size - 1];
	heap->size--;
	if (heap->size > 0)
		rearrange_heap(heap, 0, mode);
	return (value);
}

void	push_node(t_heap *heap, int burnout_time, int coder_id, int mode)
{
	heap->tab[heap->size].coder_id = coder_id;
	heap->tab[heap->size].burnout_time = burnout_time;
	heap->tab[heap->size].arrival = get_current_time();
	heap->size++;
	organise_heap(heap, heap->size - 1, mode);
}

void	clean_heap(t_heap *heap, int coder_id, int mode)
{
	int			index;

	index = 0;
	while(index < heap->size)
	{
		if (heap->tab[index].coder_id == coder_id)
			break;
		index++;
	}
	if (heap->size == 0 || index == heap->size)
		return ;
	heap->tab[index] = heap->tab[heap->size - 1];
	heap->size--;
	if (heap->size > 0)
	{
		organise_heap(heap, index, mode);
		rearrange_heap(heap, index, mode);
	}
	return ;
}
