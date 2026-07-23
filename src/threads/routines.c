/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 08:33:42 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 08:33:42 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compiling(t_coder *coder)
{
	if (coder->data->burnout_detected)
		return (0);
	coder->last_compile = timestamp(coder->data);
	display_state("is compiling", coder);
	ft_usleep(coder->data->compile, coder->data);
	if (coder->data->burnout_detected)
		return (0);
	coder->nb_compiled++;
	return (1);
}

int	debugging_and_refactoring(t_coder *coder)
{
	if (coder->data->burnout_detected)
		return (0);
	display_state("is debugging", coder);
	ft_usleep(coder->data->debug, coder->data);
	if (coder->data->burnout_detected)
		return (0);
	display_state("is refactoring", coder);
	ft_usleep(coder->data->refactor, coder->data);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (!coder->data->burnout_detected
		&& coder->nb_compiled < coder->data->required_compiles)
	{
		if (!take_both_dongles(coder))
			break ;
		if (!compiling(coder))
		{
			release_both_dongles(coder);
			break ;
		}
		release_both_dongles(coder);
		if (!debugging_and_refactoring(coder))
			break ;
	}
	return (coder);
}

void	*monitor_routine(void	*arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	return (coder);
}
