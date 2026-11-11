/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 16:35:40 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 19:36:00 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_time(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].last_compile_start = data->start_time;
		data->dongles[i].available_at = data->start_time;
		i++;
	}
}

int	init_threads(t_data *data)
{
	int	i;

	init_time(data);
	if (pthread_create(&data->printer, NULL, printer_routine, data) != 0)
		return (0);
	data->print_thread_init = 1;
	if (pthread_create(&data->req_manager, NULL,
			req_manager_routine, data) != 0)
		return (0);
	data->rman_thread_init = 1;
	i = 0;
	while (i < data->nb_coders)
	{
		if (pthread_create(&data->coders[i].thread,
				NULL, coder_routine, &data->coders[i]) != 0)
			return (0);
		data->cod_thread_init++;
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (0);
	data->mon_thread_init = 1;
	return (1);
}
