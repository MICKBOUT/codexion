/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:22:37 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 15:17:47 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_bot_dongle_available(t_coder coder)
{
	pthread_mutex_lock(&coder.left_dongle->lock_available);
	coder.left_dongle->available = 1;
	pthread_mutex_unlock(&coder.left_dongle->lock_available);
	pthread_mutex_lock(&coder.right_dongle->lock_available);
	coder.right_dongle->available = 1;
	pthread_mutex_unlock(&coder.right_dongle->lock_available);
}

void	mutex_lock_dongle_coder(t_coder coder)
{
	if (coder.left_dongle->number < coder.right_dongle->number)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
	pthread_mutex_lock(&coder.right_dongle->lock_available);
	if (coder.left_dongle->number > coder.right_dongle->number)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
}

long	get_time_since_start(t_global *global_data)
{
	struct timeval	current;
	long			time_us;

	gettimeofday(&current, NULL);
	time_us = current.tv_sec * 1000000 + current.tv_usec;
	return ((time_us / 1000) - global_data->start_time);
}
