/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/14 13:13:41 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	exit_error_parsing(void)
{
	printf("Parsing Error\n");
	return (0);
}

int	exit_free_ptr(void *ptr1, void *ptr2)
{
	if (ptr1)
		free(ptr1);
	if (ptr2)
		free(ptr2);
	return (0);
}

t_dongle	*init_dongle_tab(int nb_coders)
{
	t_dongle	*dongle_tab;
	int			i;

	dongle_tab = malloc(sizeof(t_dongle) * nb_coders);
	if (!dongle_tab)
		return (NULL);
	i = 0;
	while (i < nb_coders)
	{
		dongle_tab[i].number = i;
		i++;
	}
	return (dongle_tab);
}

t_coder	*init_coder_tab(int nb_coders, t_dongle *dongle_tab)
{
	t_coder	*coder_tab;
	int		i;

	coder_tab = malloc(sizeof(t_coder) * nb_coders);
	if (!dongle_tab || !coder_tab)
		return (NULL);
	i = 0;
	while (i < nb_coders)
	{
		coder_tab[i].number = i + 1;
		coder_tab[i].left_dongle = &(dongle_tab[i]);
		coder_tab[i].right_dongle = &(dongle_tab[(i + 1) % nb_coders]);
		i++;
	}
	return (coder_tab);
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;
	t_arg		arg;

	if ((ac != 9) || (!parsing_arg(av + 1, &arg)))
		return (exit_error_parsing());
	dongle_tab = init_dongle_tab(arg.number_of_coders);
	coder_tab = init_coder_tab(arg.number_of_coders, dongle_tab);
	if ((!coder_tab) || (!dongle_tab))
		return (exit_free_ptr(coder_tab, dongle_tab));
	for (int j = 0; j < arg.number_of_coders; j++)
		printf("coder_nb:%d|left_pt:%p|left_nb:%d|right_pt:%p|right_nb:%d|\n", coder_tab[j].number, coder_tab[j].left_dongle, coder_tab[j].left_dongle->number, coder_tab[j].right_dongle, coder_tab[j].right_dongle->number);
	return (exit_free_ptr(coder_tab, dongle_tab));
}
