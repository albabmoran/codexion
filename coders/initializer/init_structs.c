/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 00:34:02 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/03 13:19:15 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_markers(t_data *data)
{
	data->start_time = 0;
	data->stop = 0;
	data->dongles_mutex_init = 0;
	data->heap_mutex_init = 0;
	data->heap_cond_init = 0;
	data->cod_thread_init = 0;
	data->cod_cond_init = 0;
	data->cod_mutex_init = 0;
	data->print_thread_init = 0;
	data->mon_thread_init = 0;
	data->rman_thread_init = 0;
}

int	init_data(t_data *data)
{
	init_markers(data);
	data->coders = malloc(sizeof(t_coder) * data->nb_coders);
	if (!data->coders)
		return (0);
	data->dongles = malloc(sizeof(t_dongle) * data->nb_coders);
	if (!data->dongles)
		return (0);
	if (pthread_mutex_init(&data->stop_mutex, NULL))
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
	return (1);
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compiles_done = 0;
		data->coders[i].last_compile_start = 0;
		data->coders[i].data = data;
		data->coders[i].auth = 0;
		if (pthread_cond_init(&data->coders[i].cond, NULL) != 0)
			return (0);
		data->cod_cond_init++;
		if (pthread_mutex_init(&data->coders[i].request_mutex, NULL) != 0)
			return (0);
		data->cod_mutex_init++;
		i++;
	}
	return (1);
}

int	init_dongles(t_data *data)
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

int	init_heap(t_data *data)
{
	data->heap.size = 0;
	data->heap.capacity = data->nb_coders;
	data->heap.scheduler = data->scheduler;
	data->heap.requests = malloc(
			sizeof(t_request) * data->heap.capacity);
	if (!data->heap.requests)
		return (0);
	if (pthread_mutex_init(&data->heap.mutex, NULL) != 0)
		return (0);
	data->heap_mutex_init = 1;
	if (pthread_cond_init(&data->heap.cond, NULL) != 0)
		return (0);
	data->heap_cond_init = 1;
	return (1);
}
