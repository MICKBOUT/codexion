/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/13 17:09:36 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int	ft_atoi_pos(const char *n)
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
	arg->scheduler = av[7];
	return (1);
}

int	free_on_exit(void *pt1, void *pt2)
{
	if (pt1)
		free(pt1);
	if (pt2)
		free(pt2);
	return (0);
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;
	t_arg		arg;
	int			i;

	if ((ac != 9) || (!parsing_arg(av + 1, &arg)))
	{
		printf("Parsing Error\n");
		return (0);
	}
	coder_tab = malloc(sizeof(t_coder) * arg.number_of_coders);
	dongle_tab = malloc(sizeof(t_dongle) * arg.number_of_coders);
	if ((!coder_tab) || (!dongle_tab))
		return (free_on_exit(coder_tab, dongle_tab));
	i = -1;
	while (++i < arg.number_of_coders)
	{
		coder_tab[i].number = i + 1;
		dongle_tab[i].number = i + 1;
	}
	return (free_on_exit(coder_tab, dongle_tab));
}
