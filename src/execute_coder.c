/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:56:15 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 13:14:53 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->lock_available);
	locked_printf(&coder->global_ptr->lock_printf, \
"%ld %d has taken a dongle\n", \
get_time_since_start(coder->global_ptr->start_time), coder->number);
}

static void	coder_compile(t_coder *coder)
{
	long			start;
	long			current;

	start = get_time_ms();
	locked_printf(&coder->global_ptr->lock_printf, "%ld %d is compiling\n", \
get_time_since_start(coder->global_ptr->start_time), coder->number);
	current = get_time_ms();
	while (current - start < (coder->global_ptr->args.time_to_compile))
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
get_time_since_start(coder->global_ptr->start_time), coder->number);
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
get_time_since_start(coder->global_ptr->start_time), coder->number);
	current = get_time_ms();
	while (current - start < (coder->global_ptr->args.time_to_refactor))
	{
		usleep(1000);
		current = get_time_ms();
	}
}

void	execute_coder(t_coder *coder)
{
	take_dongle(coder, coder->left_dongle);
	take_dongle(coder, coder->right_dongle);
	if (!coder->global_ptr->status)
		return ;
	coder_compile(coder);
	if (!coder->global_ptr->status)
		return ;
	coder_debug(coder);
	if (!coder->global_ptr->status)
		return ;
	coder_refactor(coder);
}
