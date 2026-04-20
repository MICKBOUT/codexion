/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:22:37 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 13:14:05 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_dongle_available_after_cooldown(t_coder *coder)
{
	long	timestamp_ms;

	timestamp_ms = get_time_ms();
	pthread_mutex_lock(&coder->left_dongle->lock_available);
	coder->left_dongle->available = 1;
	coder->left_dongle->end_cooldown = \
timestamp_ms + coder->global_ptr->args.dongle_cooldown;
	pthread_mutex_unlock(&coder->left_dongle->lock_available);
	pthread_mutex_lock(&coder->right_dongle->lock_available);
	coder->right_dongle->available = 1;
	coder->right_dongle->end_cooldown = \
timestamp_ms + coder->global_ptr->args.dongle_cooldown;
	pthread_mutex_unlock(&coder->right_dongle->lock_available);
}

void	mutex_lock_dongle_coder(t_coder coder)
{
	if (coder.left_dongle->number < coder.right_dongle->number)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
	pthread_mutex_lock(&coder.right_dongle->lock_available);
	if (coder.left_dongle->number > coder.right_dongle->number)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
}

void	locked_printf(pthread_mutex_t *lock, const char *fmt, ...)
{
	va_list	args;

	pthread_mutex_lock(lock);
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	pthread_mutex_unlock(lock);
}

int	dongle_unavailable(t_dongle *l_dongle, t_dongle *r_dongle)
{
	long	timestamp_ms;

	timestamp_ms = get_time_ms();
	if (l_dongle->available == 0 || l_dongle->end_cooldown >= timestamp_ms)
		return (1);
	if (r_dongle->available == 0 || r_dongle->end_cooldown >= timestamp_ms)
		return (1);
	return (0);
}
