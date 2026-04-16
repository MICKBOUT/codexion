/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:22:37 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 14:30:39 by mboutte          ###   ########.fr       */
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
