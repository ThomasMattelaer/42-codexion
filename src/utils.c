/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmattela <tmattela@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-22 11:53:49 by tmattela          #+#    #+#             */
/*   Updated: 2026-07-22 11:53:49 by tmattela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	min(int a, int b)
{
	if (a > b)
		return (b);
	return (a);
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}
void	display_state(char *s, t_coder *coder)
{
	char	*color;
	int		index;
	int		timestamps;

	index = coder->id + 1;
	timestamps = get_current_time() - coder->data->start_time;
	if (strcmp("is compiling", s) == 0)
		color = "38;2;34;197;34";
	else if (strcmp("is debugging", s) == 0)
		color = "94";
	else if (strcmp("has taken a dongle", s) == 0)
		color = "38;2;251;191;36";
	else if (strcmp("is refactoring", s) == 0)
		color = "95";
	else if (strcmp("burned out", s) == 0)
		color = "91";
	else
		color = 0;
	printf("\x1b[90m[%dms]\x1b[0m \x1b[%sm%s\x1b[0m C%d\n",
		timestamps, color, s, index);
}
