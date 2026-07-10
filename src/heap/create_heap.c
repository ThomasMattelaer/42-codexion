/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <<tmattela@student.42belgium.b    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 11:01:20 by tmattela          #+#    #+#             */
/*   Updated: 2026/07/10 15:01:19 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*create_heap(int nb_coder)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->tab = malloc(sizeof(t_request) * nb_coder);
	if (!heap->tab)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	return (heap);
}

int	duplicate_heap(t_heap heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap.size)
	{
		if (heap.tab[i].coder_id == coder_id)
			return (1);
	}
	return (0);
}

void	organise_heap(t_heap *heap, int mode)
{
	int			curr;
	int			parent;
	t_request	tmp;
	t_request	tab;

	curr = heap->size - 1;
	parent = (curr - 1) / 2;
	while (is_higher_priority(heap->tab[curr], heap->tab[parent], mode)
		&& curr > 0)
	{
		tmp = heap->tab[parent];
		heap->tab[parent] = heap->tab[curr];
		heap->tab[curr] = tmp;
		curr = parent;
		parent = (curr - 1) / 2;
	}
}


