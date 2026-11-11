/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_manager_tasks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 20:42:19 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 21:12:37 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	authorize_coder(t_request *req, t_data *data)
{
	pthread_mutex_lock(&data->heap.mutex);
	if (data->stop)
	{
		pthread_mutex_unlock(&data->heap.mutex);
		return (0);
	}
	pthread_mutex_lock(&req->coder->request_mutex);
	req->coder->auth = 1;
	pthread_cond_broadcast(&req->coder->cond);
	pthread_mutex_unlock(&req->coder->request_mutex);
	pthread_mutex_unlock(&data->heap.mutex);
	return (1);
}

int	collect_requests(t_data *data, t_request *requests, int *count)
{
	int	i;

	pthread_mutex_lock(&data->heap.mutex);
	while (data->heap.size == 0 && !data->stop)
		pthread_cond_wait(&data->heap.cond, &data->heap.mutex);
	if (data->stop)
	{
		pthread_mutex_unlock(&data->heap.mutex);
		return (0);
	}
	*count = data->heap.size;
	i = 0;
	while (i < *count)
	{
		if (!heap_pop(&data->heap, &requests[i]))
		{
			pthread_mutex_unlock(&data->heap.mutex);
			return (-1);
		}
		i++;
	}
	pthread_mutex_unlock(&data->heap.mutex);
	return (1);
}

int	process_requests(t_data *data, t_request *requests,
		int count, int *pending)
{
	int	i;
	int	granted;

	*pending = 0;
	granted = 0;
	i = 0;
	while (i < count)
	{
		if (grant_dongles(&requests[i], data))
		{
			if (!authorize_coder(&requests[i], data))
			{
				cancel_dongles(requests[i].coder);
				return (-1);
			}
			granted++;
		}
		else
			requests[(*pending)++] = requests[i];
		i++;
	}
	return (granted);
}

int	requeue_requests(t_data *data, t_request *requests,
		int pending_count, int granted_count)
{
	int	i;
	int	new_requests;

	pthread_mutex_lock(&data->heap.mutex);
	if (data->stop)
	{
		pthread_mutex_unlock(&data->heap.mutex);
		return (0);
	}
	i = 0;
	while (i < pending_count)
	{
		if (!heap_push(&data->heap, requests[i]))
		{
			pthread_mutex_unlock(&data->heap.mutex);
			return (-1);
		}
		i++;
	}
	new_requests = (data->heap.size > pending_count);
	if (granted_count == 0 && !new_requests)
		wait_manager(data, pending_count);
	pthread_mutex_unlock(&data->heap.mutex);
	return (1);
}
