/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:25:02 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/02 01:36:39 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	memset((void *)data, 0, sizeof(t_data));
	if (!parse_args(argc, argv, data))
		return (cleanup(data), 1);
	if (!initializer(data))
		return (cleanup(data), 1);
	if (!init_threads(data))
		return (stop_simulation(data), end_threads(data), cleanup(data), 1);
	return (end_threads(data), cleanup(data), 0); /*luego ver bien que acaben los hilos y eso*/
}
