/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 18:42:51 by albben-a          #+#    #+#             */
/*   Updated: 2026/10/26 20:14:53 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_data(t_data *data)
{
	data->start_time = 0;
	data->stop = 0;
	data->coders = malloc(sizeof(t_coder) * data->nb_coders);
	if (!data->coders)
		return (0);
	data->dongles = malloc(sizeof(t_dongle) * data->nb_coders);
	if (!data->dongles)
		return (0);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (0);
	data->stop_mutex_init = 1;
	data->logs.head = NULL;
	data->logs.tail = NULL;
	data->logs.closed = 0;
	if (pthread_mutex_init(&data->logs.mutex, NULL) != 0)
		return (0);
	data->logs_mutex_init = 1;
	if (pthread_cond_init(&data->logs.cond, NULL) != 0)
		return (0);
	data->logs_cond_init = 1;
	data->dongles_mutex_init = 0;
	data->heap_mutex_init = 0;
	return (1);
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
	return ;
}

static int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].available_at = 0;
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (0);
		data->dongles_mutex_init++;
		i++;
	}
	return (1);
}

static int	init_heap(t_data *data)
{
	data->heap.size = 0;
	data->heap.capacity = data->nb_coders;
	data->heap.scheduler_type = data->scheduler;
	data->heap.requests = malloc(
			sizeof(t_request) * data->heap.capacity);
	if (!data->heap.requests)
		return (0);
	if (pthread_mutex_init(&data->heap.mutex, NULL) != 0)
		return (0);
	data->heap_mutex_init++;
	return (1);
}

int	initializer(t_data *data)
{
	if (!init_data(data))
		return (0);
	init_coders(data);
	if (!init_dongles(data))
		return (0);
	if (!init_heap(data))
		return (0);
	return (1);
}
