/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 18:42:51 by albben-a          #+#    #+#             */
/*   Updated: 2026/10/25 19:12:51 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static uint64_t get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

static int	init_data(t_data *data)
{
	data->start_time = get_time();
	data->stop = 0;
	data->coders = malloc(sizeof(t_coder) * data->nb_coders);
	if (!data->coders)
		return(0);
	data->dongles = malloc(sizeof(t_dongle) * data->nb_coders);
	if (!data->dongles)
		return (free(data->coders), 0);
	data->stop_mutex;
	data->logs;
}

static void	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compiles_done = 0;
		data->coders[i].last_compile_start = 0;
		data->coders[i].data = data;
		i++;
	}
}

static void	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].available_at = 0;
		data->dongles[i].mutex;
	}
}

void	initializer(t_data *data)
{
	if (!init_data(data))
		return ;
	init_coders(data);
	init_dongles(data);
}