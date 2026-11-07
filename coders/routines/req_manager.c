/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 02:51:42 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/07 03:19:40 by albben-a         ###   ########.fr       */
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

static int	grant_dongles(t_request *req, t_data *data)
{
	t_dongle	*right;
	t_dongle	*left;
	uint64_t	time;
	int			flag;

	flag = 0;
	if (data->nb_coders == 1)
		return (flag);
	left = &data->dongles[req->coder->id - 1];
	right = &data->dongles[req->coder->id % data->nb_coders];
	if (right->id < left->id)
	{
		pthread_mutex_lock(&right->mutex);
		pthread_mutex_lock(&left->mutex);
	}
	else
	{
		pthread_mutex_lock(&left->mutex);
		pthread_mutex_lock(&right->mutex);
	}
	if (!left->in_use && !right->in_use)
	{
		time = get_time();
		if (left->available_at <= time && right->available_at <= time)
		{
			left->in_use = 1;
			right->in_use = 1;
			flag = 1;
		}
	}
	pthread_mutex_unlock(&right->mutex);
	pthread_mutex_unlock(&left->mutex);
	return (flag);
}

void	*req_manager_routine(void *data_void)
{
	t_data		*data;
	t_request	req;
	int			i;
	int			requests_to_check;

	data = data_void;
	pthread_mutex_lock(&data->heap.mutex);
	requests_to_check = data->heap.size;
	pthread_mutex_unlock(&data->heap.mutex);
	i = 0;
	while (i < requests_to_check)
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
		if (!grant_dongles(&req, data))
		{
			pthread_mutex_lock(&data->heap.mutex);
			if (!heap_push(&data->heap, req))
			{
				pthread_mutex_unlock(&data->heap.mutex);
				stop_simulation(data);
				return (NULL);
			}
			pthread_mutex_unlock(&data->heap.mutex);
		}
		else
		{
			pthread_mutex_lock(&req.coder->request_mutex);
			req.coder->auth = 1;
			pthread_cond_broadcast(&req.coder->cond);
			pthread_mutex_unlock(&req.coder->request_mutex);
		}
		i++;
	}
	return (NULL);
}
