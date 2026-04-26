/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 13:16:08 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/26 14:12:42 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_timestamp(t_coder *coder_tab, t_global *g_data)
{
	long	time;
	long	deadline;
	int		i;

	time = get_time_ms();
	g_data->start_time = time;
	deadline = time + g_data->args.time_to_burnout;
	i = -1;
	while (++i < g_data->args.number_of_coders)
		mutex_write_burnout_time(coder_tab + i, deadline);
}

void	set_dongle_available_after_cooldown(t_coder *coder)
{
	long	timestamp_ms;

	timestamp_ms = get_time_ms();
	pthread_mutex_lock(&coder->left_dongle->lock);
	coder->left_dongle->available = 1;
	coder->left_dongle->end_cooldown = \
timestamp_ms + coder->global_ptr->args.dongle_cooldown;
	pthread_mutex_unlock(&coder->left_dongle->lock);
	pthread_mutex_lock(&coder->right_dongle->lock);
	coder->right_dongle->available = 1;
	coder->right_dongle->end_cooldown = \
timestamp_ms + coder->global_ptr->args.dongle_cooldown;
	pthread_mutex_unlock(&coder->right_dongle->lock);
}

void	set_variable(t_coder *coder_tab, t_dongle *dongle_tab)
{
	t_global	*g_data;
	int			nb_coders;
	int			i;

	g_data = coder_tab->global_ptr;
	nb_coders = g_data->args.number_of_coders;
	i = 0;
	while (i < nb_coders)
	{
		dongle_tab[i].left = &(coder_tab[mod(i - 1, nb_coders)]);
		dongle_tab[i].right = &(coder_tab[i]);
		i++;
	}
	i = -1;
	while ((++i * 2) < g_data->args.number_of_coders)
		add_coder(&(coder_tab[i * 2]));
	i = -1;
	while (((++i * 2) + 1) < g_data->args.number_of_coders)
		add_coder(&(coder_tab[(i * 2) + 1]));
}
