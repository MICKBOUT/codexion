/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:25:44 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/21 16:09:31 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_global	init_g_data(t_arg args)
{
	t_global		g_data;

	if (pthread_mutex_init(&g_data.lock_printf, NULL) != 0)
		return (g_data);
	if (pthread_mutex_init(&g_data.lock_state, NULL) != 0)
	{
		pthread_mutex_destroy(&g_data.lock_printf);
		return (g_data);
	}
	if (pthread_mutex_init(&g_data.queue.lock, NULL))
	{
		pthread_mutex_destroy(&g_data.lock_printf);
		pthread_mutex_destroy(&g_data.lock_state);
		return (g_data);
	}
	g_data.threads = malloc(\
sizeof(pthread_t) * (args.number_of_coders + 1));
	g_data.queue.head = NULL;
	g_data.args = args;
	g_data.state = 1;
	g_data.start_time = get_time_ms();
	return (g_data);
}

static void	free_mutex(int nb, t_dongle *dongle_tab)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_mutex_destroy(&dongle_tab[i].lock_available);
		i++;
	}
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
		if (pthread_mutex_init(&dongle_tab[i].lock_available, NULL) != 0)
		{
			free_mutex(i, dongle_tab);
			free(dongle_tab);
			return (NULL);
		}
		dongle_tab[i].end_cooldown = 0;
		dongle_tab[i].id = i;
		dongle_tab[i].available = 1;
		i++;
	}
	return (dongle_tab);
}

// fix the mutext init error like in the functio above
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
		pthread_mutex_init(&coder_tab[i].lock, NULL);
		coder_tab[i].global_ptr = g_data;
		coder_tab[i].id = i + 1;
		coder_tab[i].left_dongle = &(dongle_tab[i]);
		coder_tab[i].right_dongle = &(dongle_tab[(i + 1) % nb_coders]);
		coder_tab[i].burnout_time = \
get_time_ms() + g_data->args.time_to_burnout;
		coder_tab[i].nb_compil = 0;
		coder_tab[i].next = NULL;
		coder_tab[i].in_queue = 0;
		i++;
	}
	return (coder_tab);
}

void	add_coder_to_dongle(t_dongle *dongle_tab, t_coder *coder_tab)
{
	t_global	*g_data;
	int			nb_coders;
	int			i;

	g_data = coder_tab[0].global_ptr;
	nb_coders = g_data->args.number_of_coders;
	i = 0;
	while (i < nb_coders)
	{
		dongle_tab[i].left = &(coder_tab[i]);
		dongle_tab[i].right = &(coder_tab[(i + 1) % nb_coders]);
		i++;
	}
}
