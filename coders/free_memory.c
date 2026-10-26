/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 18:59:29 by albben-a          #+#    #+#             */
/*   Updated: 2026/10/26 20:11:39 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->dongles)
	{
		i = 0;
		while (i < data->dongles_mutex_init)
		{
			pthread_mutex_destroy(&data->dongles[i].mutex);
			i++;
		}
		free(data->dongles);
	}
	if (data->logs_cond_init)
		pthread_cond_destroy(&data->logs.cond);
	if (data->logs_mutex_init)
		pthread_mutex_destroy(&data->logs.mutex);
	if (data->stop_mutex_init)
		pthread_mutex_destroy(&data->stop_mutex);
	if (data->heap_mutex_init)
		pthread_mutex_destroy(&data->heap.mutex);
	free(data->heap.requests);
	free(data->coders);
	free(data);
}
