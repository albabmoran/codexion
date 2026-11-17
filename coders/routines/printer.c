/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 00:43:38 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/17 16:42:45 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	log_event_and_close(t_data *data, int coder_id,
		const char *message)
{
	t_log	*log;
	int		accepted;

	log = malloc(sizeof(t_log));
	pthread_mutex_lock(&data->logs.mutex);
	accepted = (!data->logs.closed && log != NULL);
	if (accepted)
	{
		log->timestamp = get_time() - data->start_time;
		log->coder_id = coder_id;
		log->message = message;
		log->next = NULL;
		if (data->logs.tail == NULL)
			data->logs.head = log;
		else
			data->logs.tail->next = log;
		data->logs.tail = log;
	}
	data->logs.closed = 1;
	pthread_cond_broadcast(&data->logs.cond);
	pthread_mutex_unlock(&data->logs.mutex);
	if (!accepted)
		free(log);
	return (accepted);
}

int	log_event(t_data *data, int coder_id, const char *message)
{
	t_log	*log;

	log = malloc(sizeof(t_log));
	if (!log)
		return (0);
	log->coder_id = coder_id;
	log->message = message;
	log->next = NULL;
	pthread_mutex_lock(&data->logs.mutex);
	if (data->logs.closed)
	{
		pthread_mutex_unlock(&data->logs.mutex);
		free(log);
		return (0);
	}
	log->timestamp = get_time() - data->start_time;
	if (data->logs.tail == NULL)
		data->logs.head = log;
	else
		data->logs.tail->next = log;
	data->logs.tail = log;
	pthread_cond_broadcast(&data->logs.cond);
	pthread_mutex_unlock(&data->logs.mutex);
	return (1);
}

void	*printer_routine(void *data_void)
{
	t_data	*data;
	t_log	*log;

	data = (t_data *)data_void;
	while (1)
	{
		pthread_mutex_lock(&data->logs.mutex);
		while (data->logs.head == NULL && !data->logs.closed)
			pthread_cond_wait(&data->logs.cond, &data->logs.mutex);
		if (data->logs.head == NULL && data->logs.closed)
		{
			pthread_mutex_unlock(&data->logs.mutex);
			break ;
		}
		log = data->logs.head;
		data->logs.head = log->next;
		if (data->logs.head == NULL)
			data->logs.tail = NULL;
		pthread_mutex_unlock(&data->logs.mutex);
		printf("%s%llu %d %s%s\n", data->coders[log->coder_id - 1].color,
			(unsigned long long)log->timestamp, log->coder_id, log->message,
			RESET);
		free(log);
	}
	return (NULL);
}
