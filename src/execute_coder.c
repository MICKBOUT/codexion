/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:56:15 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/21 13:11:14 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->lock_available);
	locked_printf(&coder->global_ptr->lock_printf, \
"%ld %d has taken a dongle\n", \
get_time_since_start(coder->global_ptr->start_time), coder->id);
}

static void	coder_do_task(\
	t_coder *coder, int set_dongle_free, long wait_time, char *task)
{
	long		start;
	long		current;
	t_global	*g_data;

	g_data = coder->global_ptr;
	start = get_time_ms();
	mutex_write_burnout_time(coder, start + g_data->args.time_to_burnout);
	locked_printf(&g_data->lock_printf, "%ld %d %s\n", \
get_time_since_start(g_data->start_time), coder->id, task);
	current = get_time_ms();
	while (current - start < (wait_time))
	{
		usleep(1000);
		current = get_time_ms();
	}
	if (set_dongle_free)
		set_dongle_available_after_cooldown(coder);
}

void	execute_coder(t_coder *coder)
{
	t_global	*g_data;

	take_dongle(coder, coder->left_dongle);
	take_dongle(coder, coder->right_dongle);
	g_data = coder->global_ptr;
	if (!mutex_read_int(&g_data->lock_state, &g_data->state))
		return ;
	coder_do_task(coder, 1, g_data->args.time_to_compile, "is compiling");
	if (!mutex_read_int(&g_data->lock_state, &g_data->state))
		return ;
	coder_do_task(coder, 0, g_data->args.time_to_debug, "is debugging");
	if (!mutex_read_int(&g_data->lock_state, &g_data->state))
		return ;
	coder_do_task(coder, 0, g_data->args.time_to_refactor, "is refactoring");
	mutex_add_int(&(coder->lock), &(coder->nb_compil), 1);
}
