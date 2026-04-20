/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:56:15 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 16:21:12 by mboutte          ###   ########.fr       */
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

static void	coder_compile(t_coder *coder)
{
	long		start;
	long		current;
	t_global	*g_data;

	g_data = coder->global_ptr;
	start = get_time_ms();
	mutex_write_burnout_time(coder, start + g_data->args.time_to_burnout);
	locked_printf(&g_data->lock_printf, "%ld %d is compiling\n", \
get_time_since_start(g_data->start_time), coder->id);
	current = get_time_ms();
	while (current - start < (g_data->args.time_to_compile))
	{
		usleep(1000);
		current = get_time_ms();
	}
	set_dongle_available_after_cooldown(coder);
}

static void	coder_debug(t_coder *coder)
{
	long			start;
	long			current;

	start = get_time_ms();
	locked_printf(&coder->global_ptr->lock_printf, "%ld %d is debugging\n", \
get_time_since_start(coder->global_ptr->start_time), coder->id);
	current = get_time_ms();
	while (current - start < (coder->global_ptr->args.time_to_debug))
	{
		usleep(1000);
		current = get_time_ms();
	}
}

static void	coder_refactor(t_coder *coder)
{
	long			start;
	long			current;

	start = get_time_ms();
	locked_printf(&coder->global_ptr->lock_printf, "%ld %d is refactoring\n", \
get_time_since_start(coder->global_ptr->start_time), coder->id);
	current = get_time_ms();
	while (current - start < (coder->global_ptr->args.time_to_refactor))
	{
		usleep(1000);
		current = get_time_ms();
	}
}

void	execute_coder(t_coder *coder)
{
	t_global	*g_data;

	take_dongle(coder, coder->left_dongle);
	take_dongle(coder, coder->right_dongle);
	g_data = coder->global_ptr;
	if (!mutex_read_int(&g_data->lock_state, &g_data->state))
		return ;
	coder_compile(coder);
	if (!mutex_read_int(&g_data->lock_state, &g_data->state))
		return ;
	coder_debug(coder);
	if (!mutex_read_int(&g_data->lock_state, &g_data->state))
		return ;
	coder_refactor(coder);
}
