/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 23:44:57 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 20:28:40 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	push_request(t_coder *coder)
{
	t_request	req;

	req.coder = coder;
	req.request_time = get_time();
	pthread_mutex_lock(&coder->request_mutex);
	req.deadline = coder->last_compile_start + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->request_mutex);
	pthread_mutex_lock(&coder->data->heap.mutex);
	if (coder->data->stop || !heap_push(&coder->data->heap, req))
	{
		pthread_mutex_unlock(&coder->data->heap.mutex);
		return (0);
	}
	pthread_cond_broadcast(&coder->data->heap.cond);
	pthread_mutex_unlock(&coder->data->heap.mutex);
	return (1);
}

static int	wait_authorization(t_coder *coder)
{
	int	auth;

	pthread_mutex_lock(&coder->request_mutex);
	while (coder->auth == 0)
		pthread_cond_wait(&coder->cond, &coder->request_mutex);
	auth = coder->auth;
	coder->auth = 0;
	pthread_mutex_unlock(&coder->request_mutex);
	return (auth == 1);
}

static int	has_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->request_mutex);
	finished = (coder->compiles_done >= coder->data->required_compiles);
	pthread_mutex_unlock(&coder->request_mutex);
	return (finished);
}

void	*coder_routine(void *coder_void)
{
	t_coder	*coder;

	coder = (t_coder *)coder_void;
	while (!is_stopped(coder->data) && !has_finished(coder))
	{
		if (!push_request(coder))
		{
			if (!is_stopped(coder->data))
				stop_simulation(coder->data);
			return (NULL);
		}
		if (!wait_authorization(coder))
			return (NULL);
		if (is_stopped(coder->data))
			return (NULL);
		if (!compile(coder))
			return (NULL);
		if (has_finished(coder))
			break ;
		if (!debug(coder))
			return (NULL);
		if (!refactor(coder))
			return (NULL);
	}
	return (NULL);
}
