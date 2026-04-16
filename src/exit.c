/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 13:20:20 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 14:13:52 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	exit_error_parsing(void)
{
	printf("Parsing Error\n");
	return (0);
}

int	exit_free_ptr(t_coder *coder_tab, t_dongle *dongle_tab, t_global global_data, int n)
{
	int	i;

	if (coder_tab)
		free(coder_tab);
	i = 0;
	if (dongle_tab)
	{
		while (i < n)
		{
			pthread_mutex_destroy(&dongle_tab[i].lock_available);
			i++;
		}
		free(dongle_tab);
	}
	if (global_data.threads)
		free(global_data.threads);
	pthread_mutex_destroy(&global_data.lock_printf);
	return (0);
}
