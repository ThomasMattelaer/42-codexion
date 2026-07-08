/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 11:01:20 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/08 11:46:11 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap *create_heap(int	nb_coder)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if(!heap)
		return (NULL);
	heap->tab = malloc(sizeof(t_request) * nb_coder);
	if(!heap->tab)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	return (heap);
}

int	duplicate_heap(t_heap heap, int	coder_id)
{
	int i;

	i = 0;
	while (i < heap.size)
	{
		if (heap.tab[i].coder_id == coder_id)
			return (1);
	}
	return (0);

}

void	push_node(t_heap *heap, int burnout_time, int coder_id)
{
	heap->tab[heap->size].coder_id = coder_id;
	heap->tab[heap->size].burnout_time = burnout_time;
	heap->tab[heap->size].arrival = get_current_time;
	heap->size++;
}

void	pop_node(t_heap *heap, int coder_id)
{
	
}


