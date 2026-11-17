/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 02:51:42 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/17 16:48:10 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	priority_request(t_heap *heap, t_request *a, t_request *b)
{
	if (heap->scheduler == FIFO)
		return (a->request_time <= b->request_time);
	else
		return (a->deadline <= b->deadline);
}

static int	manage_requests(t_data *data, t_request *requests)
{
	int	count;
	int	pending;
	int	granted;
	int	status;

	status = collect_requests(data, requests, &count);
	if (status <= 0)
		return (status);
	granted = process_requests(data, requests, count, &pending);
	if (granted < 0)
		return (0);
	return (requeue_requests(data, requests, pending, granted));
}

void	*req_manager_routine(void *data_void)
{
	t_data		*data;
	t_request	*requests;
	int			status;

	data = (t_data *)data_void;
	requests = malloc(sizeof(t_request) * data->nb_coders);
	if (!requests)
		return (stop_simulation(data), NULL);
	status = 1;
	while (status > 0)
		status = manage_requests(data, requests);
	free(requests);
	if (status < 0)
		stop_simulation(data);
	return (NULL);
}
