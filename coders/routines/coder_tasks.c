/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 20:24:06 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 20:29:33 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	begin_compile(t_coder *coder)
{
	t_data	*data;

	data = coder->data;
	pthread_mutex_lock(&data->heap.mutex);
	if (data->stop)
	{
		pthread_mutex_unlock(&data->heap.mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->request_mutex);
	coder->last_compile_start = get_time();
	pthread_mutex_unlock(&coder->request_mutex);
	if (!log_event(data, coder->id, "has taken a dongle")
		|| !log_event(data, coder->id, "has taken a dongle")
		|| !log_event(data, coder->id, "is compiling"))
	{
		pthread_mutex_unlock(&data->heap.mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->heap.mutex);
	return (1);
}

int	compile(t_coder *coder)
{
	if (!begin_compile(coder))
	{
		cancel_dongles(coder);
		if (!is_stopped(coder->data))
			stop_simulation(coder->data);
		return (0);
	}
	if (!wait_time(coder, coder->data->time_to_compile))
	{
		release_dongles(coder);
		return (0);
	}
	pthread_mutex_lock(&coder->request_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->request_mutex);
	release_dongles(coder);
	return (1);
}

int	debug(t_coder *coder)
{
	if (!log_event(coder->data, coder->id, "is debugging"))
	{
		if (!is_stopped(coder->data))
			stop_simulation(coder->data);
		return (0);
	}
	return (wait_time(coder, coder->data->time_to_debug));
}

int	refactor(t_coder *coder)
{
	if (!log_event(coder->data, coder->id, "is refactoring"))
	{
		if (!is_stopped(coder->data))
			stop_simulation(coder->data);
		return (0);
	}
	return (wait_time(coder, coder->data->time_to_refactor));
}
