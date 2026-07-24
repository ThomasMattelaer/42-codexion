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

	index = coder->id;
	if (strcmp("is compiling", s) == 0)
		color = "38;2;34;197;34";
	else if (strcmp("is debugging", s) == 0)
		color = "94";
	else if (strcmp("is refactoring", s) == 0)
		color = "95";
	else if (strcmp("burned out", s) == 0)
		color = "91";
	else
		color = 0;
	pthread_mutex_lock(&coder->data->write_mutex);
	printf("\x1b[90m[%lldms]\x1b[0m \x1b[%sm%s\x1b[0m C%d\n",
		timestamp(coder->data), color, s, index);
	pthread_mutex_unlock(&coder->data->write_mutex);
}

void	display_dongle(char *s, t_coder *coder, int nb)
{
	char	*color;
	int		index;

	index = coder->id;
	if (strcmp("request a dongle", s) == 0)
		color = "38;2;251;191;36";
	else if (strcmp("release a dongle", s) == 0)
		color = "38;2;148;163;184";
	else if (strcmp("has taken a dongle", s) == 0)
		color = "38;2;16;185;129";
	else
		color = 0;
	pthread_mutex_lock(&coder->data->write_mutex);
	printf("\x1b[90m[%lldms]\x1b[0m \x1b[%sm%s\x1b[0m C%d D%d\n",
		timestamp(coder->data), color, s, index, nb);
	pthread_mutex_unlock(&coder->data->write_mutex);
}
int	burnout_detected(t_data *data)
{
	int	burnout;

	pthread_mutex_lock(&data->burn_mutex);
	burnout = data->burnout_detected;
	pthread_mutex_unlock(&data->burn_mutex);
	if (burnout)
		return (1);
	return (0);
}
