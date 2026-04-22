/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:22:37 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/22 15:18:04 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	locked_printf(t_global *g_data, int is_burnout, int id, char *task)
{
	pthread_mutex_lock(&g_data->lock_printf);
	if ((g_data->stop_printf) == 0)
	{
		printf("%ld %d %s\n", \
get_time_since_start(g_data->start_time), id, task);
		if (is_burnout)
			g_data->stop_printf = 1;
	}
	pthread_mutex_unlock(&g_data->lock_printf);
}

int	dongle_available(t_dongle *l_dongle, t_dongle *r_dongle)
{
	long	timestamp_ms;

	timestamp_ms = get_time_ms();
	if (l_dongle->available == 0 || l_dongle->end_cooldown > timestamp_ms)
		return (0);
	if (r_dongle->available == 0 || r_dongle->end_cooldown > timestamp_ms)
		return (0);
	return (1);
}

int	mod(int nb, int m)
{
	if (nb < m)
		nb += m;
	return (nb % m);
}

void	wait_for_start(t_global *g_data)
{
	while (mutex_read_int(&g_data->lock_state, &g_data->state) == 2)
		usleep(100);
}
