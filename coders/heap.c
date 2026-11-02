/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 03:39:42 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/02 05:57:35 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	heap_up(t_heap *heap, int idx)
{
	while (idx > 0 && priority_request(heap, &heap->requests[idx],
			&heap->requests[(idx - 1) / 2]))
	{
		swap_requests(&heap->requests[idx], &heap->requests[(idx - 1) / 2]);
		idx = (idx - 1) / 2;
	}
}

static void	heap_down(t_heap *heap, int idx)
{
	int	left;
	int	right;
	int	temp;

	while (idx < heap->size)
	{
		left = 2 * idx + 1;
		right = 2 * idx + 2;
		temp = idx;
		if (left < heap->size)
		{
			if (priority_request(heap, &heap->requests[left],
					&heap->requests[temp]))
				temp = left;
		}
		if (right < heap->size)
		{
			if (priority_request(heap, &heap->requests[right],
					&heap->requests[temp]))
				temp = right;
		}
		if (idx == temp)
			break ;
		swap_requests(&heap->requests[temp], &heap->requests[idx]);
		idx = temp;
	}
}

int	heap_pop(t_heap *heap, t_request *req)
{
	if (heap->size == 0)
		return (0);
	*req = heap->requests[0];
	heap->requests[0] = heap->requests[heap->size - 1];
	heap->size--;
	if (heap->size)
		heap_down(heap, 0);
	return (1);
}

int	heap_push(t_heap *heap, t_request a)
{
	int	idx;

	idx = heap->size;
	if (idx < heap->capacity)
	{
		heap->requests[idx] = a;
		heap->size++;
		heap_up(heap, idx);
		return (1);
	}
	return (0);
}
