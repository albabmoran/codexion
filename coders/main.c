/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:25:02 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/11 21:19:55 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	memset(data, 0, sizeof(t_data));
	if (!parse_args(argc, argv, data))
		return (cleanup(data), 1);
	if (!initializer(data))
		return (cleanup(data), 1);
	if (!init_threads(data))
		return (stop_simulation(data), end_threads(data), cleanup(data), 1);
	end_threads(data);
	/*int i = 0;

	while (i < data->nb_coders)
	{
		printf("Coder %d: %d/%d compiles\n",
			data->coders[i].id,
			data->coders[i].compiles_done,
			data->required_compiles);
		i++;
	}
	printf("SIMULATION FINISHED SUCCESSFULLY\n");*/
	cleanup(data);
	return (0);
}
