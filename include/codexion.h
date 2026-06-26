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

typedef struct s_data
{
	int	nb_coders;
	int	burnout;
	int	compile;
	int	debug;
	int	refactor;
	int	required;
	int dongles;
	int	FIFO;
	int	EDF;
}	t_data; 

int	is_valid_data(int argc, char **argv);
int	is_enough_arguments(int argc);

#endif
