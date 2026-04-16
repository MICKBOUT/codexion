/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:25:44 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 17:25:04 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_global	init_global_data(t_arg args)
{
	t_global		global_data;
	struct timeval	time;
	long			start_time_us;

	pthread_mutex_init(&global_data.lock_printf, NULL);
	global_data.threads = malloc(sizeof(pthread_t) * args.number_of_coders);
	global_data.args = args;
	gettimeofday(&time, NULL);
	start_time_us = time.tv_sec * 1000000 + time.tv_usec;
	global_data.start_time = (start_time_us / 1000);
	return (global_data);
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
		pthread_mutex_init(&dongle_tab[i].lock_available, NULL);
		dongle_tab[i].end_cooldown = 0;
		dongle_tab[i].number = i;
		dongle_tab[i].available = 1;
		i++;
	}
	return (dongle_tab);
}

t_coder	*init_coder_tab(int nb_coders, t_dongle *dongle_tab, t_global *global)
{
	t_coder	*coder_tab;
	int		i;

	coder_tab = malloc(sizeof(t_coder) * nb_coders);
	if (!dongle_tab || !coder_tab)
		return (NULL);
	i = 0;
	while (i < nb_coders)
	{
		coder_tab[i].global_ptr = global;
		coder_tab[i].number = i + 1;
		coder_tab[i].left_dongle = &(dongle_tab[i]);
		coder_tab[i].right_dongle = &(dongle_tab[(i + 1) % nb_coders]);
		i++;
	}
	return (coder_tab);
}
