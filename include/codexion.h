/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-24 12:17:48 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-24 12:17:48 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct	s_coder
{
	int		thread_id;
	t_data	*data
}	t_coder;

typedef struct	s_data
{
	int	nb_coders;
	int	burnout;
	int	compile;
	int	debug;
	int	refactor;
	int	required;
	int	cooldown;
	int	scheduler;
}	t_data;

int	is_valid_data(int argc, char **argv);
int	is_enough_arguments(int argc);
int valid_scheduler(char *s1);

#endif
