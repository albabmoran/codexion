/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 02:51:42 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/04 03:37:18 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	priority_request(t_heap *heap, t_request *a, t_request *b)
{
	if (heap->scheduler == FIFO)
		return (a->request_time <= b->request_time);
	else
		return (a->deadline <= b->deadline); /*1 si a tiene prioridad, 0 si no*/
}

static int	is_stopped(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->heap.mutex);
	stop = data->stop;
	pthread_mutex_unlock(&data->heap.mutex);
	return (stop);
}

void	*req_manager_routine(void *data_void)
{
	t_data		*data;
	t_request	req;

	data = data_void;
	while (1)
	{
		pthread_mutex_lock(&data->heap.mutex);
		while (data->heap.size == 0 && !data->stop)
			pthread_cond_wait(&data->heap.cond, &data->heap.mutex);
		if (data->stop)
			return (pthread_mutex_unlock(&data->heap.mutex), NULL);
		if (!heap_pop(&data->heap, &req))
		{
			pthread_mutex_unlock(&data->heap.mutex);
			stop_simulation(data);
			return (NULL);
		}
		pthread_mutex_unlock(&data->heap.mutex);
		pthread_mutex_lock(&req.coder->request_mutex);
		req.coder->auth = 1;
		pthread_cond_broadcast(&req.coder->cond);
		pthread_mutex_unlock(&req.coder->request_mutex);
	}
	return (NULL);
}
