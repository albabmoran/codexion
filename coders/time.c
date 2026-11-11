/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 20:10:06 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 20:33:38 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

int	wait_time(t_coder *coder, uint64_t duration)
{
	uint64_t	start;

	start = get_time();
	while (get_time() - start < duration)
	{
		if (is_stopped(coder->data))
			return (0);
		usleep(500);
	}
	return (1);
}

void	wait_manager(t_data *data, int pending_count)
{
	struct timeval	tv;
	struct timespec	ts;

	if (pending_count == 0)
	{
		pthread_cond_wait(&data->heap.cond, &data->heap.mutex);
		return ;
	}
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = (tv.tv_usec * 1000) + 1000000;
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	pthread_cond_timedwait(&data->heap.cond, &data->heap.mutex, &ts);
}
