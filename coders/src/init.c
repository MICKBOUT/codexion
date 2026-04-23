/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:25:44 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/23 16:17:38 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*free_mutex(int nb, void *ell, int is_dongle)
{
	t_dongle	*dongle_tab;
	t_coder		*coder_tab;
	int			i;

	i = -1;
	if (is_dongle)
	{
		coder_tab = (t_coder *)ell;
		while (++i < nb)
			pthread_mutex_destroy(&coder_tab[i].lock);
		free(coder_tab);
	}
	else
	{
		dongle_tab = (t_dongle *)ell;
		while (++i < nb)
			pthread_mutex_destroy(&dongle_tab[i].lock);
		free(dongle_tab);
	}
	return (NULL);
}

t_global	free_threads(t_global g_data, int n)
{
	if (n >= 1)
		pthread_mutex_destroy(&g_data.lock_printf);
	if (n >= 2)
		pthread_mutex_destroy(&g_data.lock_state);
	free(g_data.threads);
	return (g_data);
}

t_global	init_g_data(t_arg args)
{
	t_global		g_data;

	g_data.queue.head = NULL;
	g_data.args = args;
	g_data.state = 2;
	g_data.stop_printf = 0;
	g_data.threads = malloc(sizeof(pthread_t) * (args.number_of_coders + 1));
	if (!g_data.threads)
		return (g_data);
	if (pthread_mutex_init(&g_data.lock_printf, NULL) != 0)
		return (free_threads(g_data, 0));
	if (pthread_mutex_init(&g_data.lock_state, NULL) != 0)
		return (free_threads(g_data, 1));
	if (pthread_mutex_init(&g_data.queue.lock, NULL) != 0)
		return (free_threads(g_data, 2));
	return (g_data);
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
		if (pthread_mutex_init(&dongle_tab[i].lock, NULL) != 0)
			return (free_mutex(i, dongle_tab, 1));
		dongle_tab[i].end_cooldown = 0;
		dongle_tab[i].id = i;
		dongle_tab[i].available = 1;
		i++;
	}
	return (dongle_tab);
}

t_coder	*init_coder_tab(int nb_coders, t_dongle *dongle_tab, t_global *g_data)
{
	t_coder	*coder_tab;
	int		i;

	coder_tab = malloc(sizeof(t_coder) * nb_coders);
	if (!dongle_tab || !coder_tab)
		return (NULL);
	i = 0;
	while (i < nb_coders)
	{
		if (pthread_mutex_init(&coder_tab[i].lock, NULL) != 0)
			return (free_mutex(i, coder_tab, 0));
		coder_tab[i].global_ptr = g_data;
		coder_tab[i].id = i + 1;
		coder_tab[i].left_dongle = &(dongle_tab[i]);
		coder_tab[i].right_dongle = &(dongle_tab[(i + 1) % nb_coders]);
		coder_tab[i].nb_compil = 0;
		coder_tab[i].next = NULL;
		coder_tab[i].in_queue = 0;
		coder_tab[i].burnout_time = 0;
		i++;
	}
	return (coder_tab);
}
