/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 20:07:12 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 20:21:30 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	block_dongles(t_dongle *left, t_dongle *right)
{
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
}

void	cancel_dongles(t_coder *coder)
{
	t_data		*data;
	t_dongle	*left;
	t_dongle	*right;

	data = coder->data;
	if (data->nb_coders == 1)
		return ;
	left = &data->dongles[coder->id - 1];
	right = &data->dongles[coder->id % data->nb_coders];
	block_dongles(right, left);
	left->in_use = 0;
	right->in_use = 0;
	pthread_mutex_unlock(&right->mutex);
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&data->heap.mutex);
	pthread_cond_broadcast(&data->heap.cond);
	pthread_mutex_unlock(&data->heap.mutex);
}

void	release_dongles(t_coder *coder)
{
	t_data		*data;
	t_dongle	*left;
	t_dongle	*right;
	uint64_t	time;

	data = coder->data;
	left = &data->dongles[coder->id - 1];
	right = &data->dongles[coder->id % data->nb_coders];
	block_dongles(right, left);
	time = get_time();
	left->in_use = 0;
	right->in_use = 0;
	left->available_at = time + data->dongle_cooldown;
	right->available_at = time + data->dongle_cooldown;
	pthread_mutex_unlock(&right->mutex);
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&data->heap.mutex);
	pthread_cond_broadcast(&data->heap.cond);
	pthread_mutex_unlock(&data->heap.mutex);
}

int	grant_dongles(t_request *req, t_data *data)
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
	block_dongles(right, left);
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
