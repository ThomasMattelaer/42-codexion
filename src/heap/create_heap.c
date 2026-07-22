/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 07:11:05 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 07:11:05 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*create_heap(int nb_coder, int mode)
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
	heap->mode = mode;
	return (heap);
}

int	find_duplicate(const t_heap *heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->tab[i].coder_id == coder_id)
			return (1);
		i++;
	}
	return (0);
}

int	is_higher_priority(t_request a, t_request b, int mode)
{
	if (mode == 2)
	{
		if (a.deadline == b.deadline)
			return (a.coder_id < b.coder_id);
		return (a.deadline < b.deadline);
	}
	return (a.arrival_time < b.arrival_time);
}

void	organise_heap(t_heap *heap, int curr)
{
	int			parent;
	t_request	tmp;

	parent = (curr - 1) / 2;
	while (curr > 0
		&& is_higher_priority(heap->tab[curr], heap->tab[parent], heap->mode))
	{
		tmp = heap->tab[parent];
		heap->tab[parent] = heap->tab[curr];
		heap->tab[curr] = tmp;
		curr = parent;
		parent = (curr - 1) / 2;
	}
}

void	rearrange_heap(t_heap *heap, int curr)
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
			&& is_higher_priority(heap->tab[right], heap->tab[left],
				heap->mode))
			smallest = right;
		if (is_higher_priority(heap->tab[smallest], heap->tab[curr],
				heap->mode))
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
