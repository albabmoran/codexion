/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 23:44:57 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/04 00:47:31 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_request	create_push_request(t_coder *coder)
{
	t_request	req;

	req.coder = coder;
	req.request_time = get_time();
	req.deadline = req.request_time + coder->data->time_to_burnout;
	heap_pus(coder->data->heap, req);
}

void	*coder_routine(void *coder)
{
	t_request	req;

	coder = ((t_coder *)coder);
	req = create_push_request(coder);
	pthread_mutex_lock(&((t_coder *)coder)->request_mutex);
	while (((t_coder *)coder)->auth == 0)
		pthread_cond_wait(&((t_coder *)coder)->cond,
			&((t_coder *)coder)->request_mutex);
	return (NULL);
}
