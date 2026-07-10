/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:59:32 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/10 15:01:12 by tmattela         ###   ########.fr       */
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

t_request	pop_node(t_heap *heap, int coder_id, int mode)
{
	t_request	value;
	int			curr;
	int			left;
	int			right;
	int			smallest;

	curr = 0;
	if (heap->size == 0)
		return ;
	value = heap->tab[0];
	heap->tab[0] = heap->tab[heap->size - 1];
	heap->size--;
	while (2 * curr + 1 < heap->size)
	{
		left = 2 * curr + 1;
		right = 2 * curr + 2;
		smallest = left;
		if (is_higher_priority(heap->tab[right],heap->tab[left], mode)
			&& right < heap->size)
			smallest = right;
		if (is_higher_priority(heap->tab[smallest], heap->tab[curr], mode))
		{

		}
	}


}

void	push_node(t_heap *heap, int burnout_time, int coder_id)
{
	heap->tab[heap->size].coder_id = coder_id;
	heap->tab[heap->size].burnout_time = burnout_time;
	heap->tab[heap->size].arrival = get_current_time;
	heap->size++;
}
