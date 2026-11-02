/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 18:42:51 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/02 01:36:01 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	initializer(t_data *data)
{
	if (!init_data(data))
		return (0);
	init_coders(data);
	if (!init_dongles(data))
		return (0);
	if (!init_heap(data))
		return (0);
	return (1);
}
