/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mutex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:47:29 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 16:20:17 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	mutex_write_int(pthread_mutex_t *lock, int *place, int var)
{
	pthread_mutex_lock(lock);
	*place = var;
	pthread_mutex_unlock(lock);
}

int	mutex_read_int(pthread_mutex_t *lock, int *place)
{
	int	res;

	pthread_mutex_lock(lock);
	res = *place;
	pthread_mutex_unlock(lock);
	return (res);
}

long	mutex_read_burnout_time(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->lock);
	time = coder->burnout_time;
	pthread_mutex_unlock(&coder->lock);
	return (time);
}

void	mutex_write_burnout_time(t_coder *coder, long time)
{
	pthread_mutex_lock(&coder->lock);
	coder->burnout_time = time;
	pthread_mutex_unlock(&coder->lock);
}

void	mutex_lock_dongle(t_coder coder)
{
	if (coder.left_dongle->id < coder.right_dongle->id)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
	pthread_mutex_lock(&coder.right_dongle->lock_available);
	if (coder.left_dongle->id > coder.right_dongle->id)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
}
