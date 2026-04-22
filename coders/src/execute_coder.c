/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:56:15 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/22 15:10:12 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_do_task(\
	t_coder *coder, int compiling, long wait_time, char *task)
{
	long		start;
	long		current;
	t_global	*g_data;

	g_data = coder->global_ptr;
	start = get_time_ms();
	if (compiling)
		mutex_write_burnout_time(coder, start + g_data->args.time_to_burnout);
	locked_printf(g_data, 0, coder->id, task);
	current = get_time_ms();
	while (current - start < (wait_time) && \
mutex_read_int(&g_data->lock_state, &g_data->state))
	{
		usleep(1000);
		current = get_time_ms();
	}
	if (compiling)
		set_dongle_available_after_cooldown(coder);
}

void	execute_coder(t_coder *coder)
{
	t_global	*g_data;

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
