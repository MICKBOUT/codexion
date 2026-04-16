/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:40:22 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 17:45:53 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec * 1000 + (current.tv_usec / 1000)));
}

long	get_time_since_start(long start_time)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((get_time_ms()) - start_time);
}
