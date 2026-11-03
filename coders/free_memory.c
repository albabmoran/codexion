/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 18:59:29 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/03 13:17:54 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->dongles_mutex_init)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	free(data->dongles);
}

static void	destroy_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->cod_cond_init)
	{
		pthread_cond_destroy(&data->coders[i].cond);
		i++;
	}
	i = 0;
	while (i < data->cod_mutex_init)
	{
		pthread_mutex_destroy(&data->coders[i].request_mutex);
		i++;
	}
	free(data->coders);
}

void	cleanup(t_data *data)
{
	if (!data)
		return ;
	if (data->dongles)
		destroy_dongles(data);
	if (data->coders)
		destroy_coders(data);
	if (data->logs_cond_init)
		pthread_cond_destroy(&data->logs.cond);
	if (data->logs_mutex_init)
		pthread_mutex_destroy(&data->logs.mutex);
	if (data->stop_mutex_init)
		pthread_mutex_destroy(&data->stop_mutex);
	if (data->heap_cond_init)
		pthread_cond_destroy(&data->heap.cond);
	if (data->heap_mutex_init)
		pthread_mutex_destroy(&data->heap.mutex);
	free(data->heap.requests);
	free(data);
}

void	end_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->cod_thread_init)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	if (data->rman_thread_init)
		pthread_join(data->req_manager, NULL);
	if (data->mon_thread_init)
		pthread_join(data->monitor, NULL);
	if (data->print_thread_init)
		pthread_join(data->printer, NULL);
}
