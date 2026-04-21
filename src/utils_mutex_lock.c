/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mutex_lock.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:48:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/21 14:35:35 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	mutex_unlock_worked(t_coder *coder)
{
	pthread_mutex_unlock(&coder->right_dongle->lock_available);
	pthread_mutex_unlock(&coder->left_dongle->lock_available);
	pthread_mutex_unlock(&coder->global_ptr->queue.lock);
}

void	mutex_lock_dongle(t_coder *coder)
{
	if (coder->left_dongle->id < coder->right_dongle->id)
		pthread_mutex_lock(&coder->left_dongle->lock_available);
	pthread_mutex_lock(&coder->right_dongle->lock_available);
	if (coder->left_dongle->id > coder->right_dongle->id)
		pthread_mutex_lock(&coder->left_dongle->lock_available);
}

void	mutex_unlock_dongle(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->lock_available);
	pthread_mutex_unlock(&coder->right_dongle->lock_available);
}
