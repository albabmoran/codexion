/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 23:44:57 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/04 01:33:31 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_push_request(t_coder *coder)
{
	t_request	req;

	req.coder = coder;
	req.request_time = get_time();
	req.deadline = req.coder->last_compile_start + coder->data->time_to_burnout;
	pthread_mutex_lock(&coder->data->heap.mutex);
	if (!heap_push(&coder->data->heap, req))
		return (0);
	pthread_cond_broadcast(&coder->data->heap.cond);
	pthread_mutex_unlock(&coder->data->heap.mutex);
	return (1);
}

void	*coder_routine(void *coder_void)
{
	t_coder	*coder;

	coder = (t_coder *)coder_void;
	if (!create_push_request(coder))
	{
		stop_simulation(coder->data);
		return (NULL);
	}
	pthread_mutex_lock(&coder->request_mutex);
	while (coder->auth == 0)
		pthread_cond_wait(&coder->cond, &coder->request_mutex);
	pthread_mutex_unlock(&coder->request_mutex);
	return (NULL);
}
