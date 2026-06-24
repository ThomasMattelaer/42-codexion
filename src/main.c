/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-24 12:17:28 by tmattela          #+#    #+#             */
/*   Updated: 2026-06-24 12:17:28 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	main(int argc, char **argv)
{
	if(is_valid_data(argv) == 0 || is_enough_arguments(argc) == 0)
		return(0);
	return (1);
}
