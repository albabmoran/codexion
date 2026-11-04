/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 01:29:11 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/04 02:26:27 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->heap.mutex);
	data->stop = 1;
	pthread_mutex_unlock(&data->heap.mutex);
}
/*modifica stop con heap.mutex, despierta a req_manager con heap.cond*/