/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 01:29:11 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 20:17:42 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_stopped(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->heap.mutex);
	stop = data->stop;
	pthread_mutex_unlock(&data->heap.mutex);
	return (stop);
}

void	complete_stop_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->coders[i].request_mutex);
		data->coders[i].auth = -1;
		pthread_cond_broadcast(&data->coders[i].cond);
		pthread_mutex_unlock(&data->coders[i].request_mutex);
		i++;
	}
	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		data->dongles[i].in_use = 0;
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_lock(&data->logs.mutex);
	data->logs.closed = 1;
	pthread_cond_broadcast(&data->logs.cond);
	pthread_mutex_unlock(&data->logs.mutex);
}

void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->heap.mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->heap.cond);
	pthread_mutex_unlock(&data->heap.mutex);
	complete_stop_simulation(data);
}
