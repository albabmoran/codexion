/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 02:51:42 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/02 06:00:44 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	priority_request(t_heap *heap, t_request *a, t_request *b) /*1 si a tiene prioridad, 0 si no*/
{
	if (heap->scheduler == FIFO)
		return (a->request_time <= b->request_time);
	else
		return (a->deadline <= b->deadline);
}

void	*req_manager_routine(void *data)
{
	data = (t_data *)data;
	return (NULL);
}
