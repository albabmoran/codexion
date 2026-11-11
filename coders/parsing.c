/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:24:52 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/09 20:38:26 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_digits(char *str)
{
	int				i;
	unsigned long	digits;

	i = 0;
	digits = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
			digits++;
		i++;
	}
	if (digits == strlen(str))
		return (1);
	else
		return (0);
}

static int	init_args(int *arg, t_data *data)
{
	data->nb_coders = arg[0];
	if (data->nb_coders == 0)
		return (0);
	data->time_to_burnout = (uint64_t)arg[1];
	data->time_to_compile = (uint64_t)arg[2];
	data->time_to_debug = (uint64_t)arg[3];
	data->time_to_refactor = (uint64_t)arg[4];
	data->required_compiles = arg[5];
	data->dongle_cooldown = (uint64_t)arg[6];
	return (1);
}

static int	check_init_scheduler(char *str, t_data *data)
{
	int	flag;

	flag = 1;
	if (!strcmp(str, "fifo"))
		data->scheduler = FIFO;
	else if (!strcmp(str, "edf"))
		data->scheduler = EDF;
	else
		flag = 0;
	return (flag);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int		flag;
	int		i;
	int		arg[7];

	flag = 1;
	if (argc != 9)
		flag = 0;
	i = 1;
	while (i < 8 && flag != 0)
	{
		if (check_digits(argv[i]))
			arg[i - 1] = atoi(argv[i]);
		else
			flag = 0;
		i++;
	}
	if (flag && !check_init_scheduler(argv[8], data))
		flag = 0;
	if (flag && !init_args(arg, data))
		flag = 0;
	return (flag);
}
