/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:20:20 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/22 10:23:02 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	exit_error_parsing(void)
{
	printf("Parsing Error\n");
	return (0);
}

int	exit_free(t_coder *coder_tab, t_dongle *dongle_tab, t_global g_data)
{
	int	i;

	if (coder_tab)
	{
		i = -1;
		while (++i < g_data.args.number_of_coders)
			pthread_mutex_destroy(&coder_tab[i].lock);
		free(coder_tab);
	}
	if (dongle_tab)
	{
		i = -1;
		while (++i < g_data.args.number_of_coders)
			pthread_mutex_destroy(&dongle_tab[i].lock);
		free(dongle_tab);
	}
	if (g_data.threads)
	{
		pthread_mutex_destroy(&g_data.lock_printf);
		pthread_mutex_destroy(&g_data.lock_state);
		pthread_mutex_destroy(&g_data.queue.lock);
		free(g_data.threads);
	}
	return (0);
}

int	error_exit_free(t_coder *coder_tab, t_dongle *dongle_tab, t_global g_data)
{
	printf("Fail in malloc and/or mutex init, try again or later.\n");
	exit_free(coder_tab, dongle_tab, g_data);
	return (0);
}
