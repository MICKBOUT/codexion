/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:56:15 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 17:58:23 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_dongle(t_coder coder, t_dongle *dongle)
{
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->lock_available);
	locked_printf(&coder.global_ptr->lock_printf, \
"%ld %d has taken a dongle\n", \
get_time_since_start(coder.global_ptr->start_time), coder.number);
}

static void	coder_compile(t_coder coder)
{
	struct timeval	time;
	long			start;
	long			current;

	gettimeofday(&time, NULL);
	locked_printf(&coder.global_ptr->lock_printf, "%ld %d is compiling\n", \
get_time_since_start(coder.global_ptr->start_time), coder.number);
	start = coder.global_ptr->start_time;
	current = time.tv_sec * 1000000 + time.tv_usec;
	while (current - start < (coder.global_ptr->args.time_to_compile * 1000))
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
	set_dongle_available_after_cooldown(coder);
}

static void	coder_debug(t_coder coder)
{
	struct timeval	time;
	long			start;
	long			current;

	gettimeofday(&time, NULL);
	locked_printf(&coder.global_ptr->lock_printf, "%ld %d is debugging\n", \
get_time_since_start(coder.global_ptr->start_time), coder.number);
	start = coder.global_ptr->start_time;
	current = time.tv_sec * 1000000 + time.tv_usec;
	while (current - start < (coder.global_ptr->args.time_to_debug * 1000))
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
}

void	execute_coder(t_coder coder)
{
	take_dongle(coder, coder.left_dongle);
	take_dongle(coder, coder.right_dongle);
	coder_compile(coder);
	coder_debug(coder);
}
