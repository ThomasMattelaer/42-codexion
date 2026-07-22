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

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while(!coder->data->burnout_detected
		&& coder->nb_compiled < coder->data->required_compiles)
	{
		if (!take_both_dongles(coder))
			continue ;
		
	}
		return (1);
	return ;
}

void	*monitor_routine(void	*arg)
{
	return ;
}
