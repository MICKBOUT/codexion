/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:24:01 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/14 13:41:24 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_atoi_pos(const char *n)
{
	long int	res;
	int			i;

	i = 0;
	if (n[0] == '-')
		return (-1);
	else if (n[0] == '+')
		i++;
	res = 0;
	while ('0' <= n[i] && n[i] <= '9')
	{
		res *= 10;
		res += n[i++] - '0';
		if (INT_MAX < res)
			return (-1);
	}
	if (n[i] != '\0' || res == 0)
		return (-1);
	return ((int) res);
}

int	parsing_arg(char **av, t_arg *arg)
{
	int	*targets[7];
	int	i;

	targets[0] = &arg->number_of_coders;
	targets[1] = &arg->time_to_burnout;
	targets[2] = &arg->time_to_compile;
	targets[3] = &arg->time_to_debug;
	targets[4] = &arg->time_to_refactor;
	targets[5] = &arg->number_of_compiles_required;
	targets[6] = &arg->dongle_cooldown;
	i = -1;
	while (++i < 7)
	{
		*targets[i] = ft_atoi_pos(av[i]);
		if (*targets[i] == -1)
			return (0);
	}
	if (strcmp(av[7], "edf") != 0 && strcmp(av[7], "fifo") != 0)
		return (0);
	arg->scheduler = av[7];
	return (1);
}
