/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 00:47:12 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/17 16:44:12 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_coder_status(t_coder *coder, t_data *data)
{
	uint64_t	last_compile;
	int			finished;
	int			burned_out;

	pthread_mutex_lock(&coder->request_mutex);
	finished = (coder->compiles_done >= data->required_compiles);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->request_mutex);
	if (finished)
		return (0);
	burned_out = (get_time() - last_compile >= data->time_to_burnout);
	return (burned_out);
}

static int	check_burnout(t_data *data)
{
	int	i;
	int	should_stop;

	i = 0;
	should_stop = 0;
	while (i < data->nb_coders)
	{
		if (check_coder_status(&data->coders[i], data))
		{
			log_event_and_close(data, data->coders[i].id, "burned out");
			data->stop = 1;
			pthread_cond_broadcast(&data->heap.cond);
			should_stop = 1;
			break ;
		}
		i++;
	}
	return (should_stop);
}

static int	all_coders_finished(t_data *data)
{
	int	i;
	int	finished;

	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->coders[i].request_mutex);
		finished = (data->coders[i].compiles_done
				>= data->required_compiles);
		pthread_mutex_unlock(&data->coders[i].request_mutex);
		if (!finished)
			return (0);
		i++;
	}
	return (1);
}

static int	check_simulation_status(t_data *data)
{
	if (data->stop)
		return (1);
	if (all_coders_finished(data))
	{
		data->stop = 1;
		pthread_cond_broadcast(&data->heap.cond);
		return (2);
	}
	return (0);
}

void	*monitor_routine(void *data_void)
{
	t_data	*data;
	int		status;

	data = (t_data *)data_void;
	while (1)
	{
		pthread_mutex_lock(&data->heap.mutex);
		status = check_simulation_status(data);
		if (status)
		{
			pthread_mutex_unlock(&data->heap.mutex);
			if (status == 2)
				complete_stop_simulation(data);
			return (NULL);
		}
		status = check_burnout(data);
		pthread_mutex_unlock(&data->heap.mutex);
		if (status)
		{
			complete_stop_simulation(data);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
