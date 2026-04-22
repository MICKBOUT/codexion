/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/22 15:10:45 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_dongles(t_coder *coder)
{
	t_dongle	*right;
	t_dongle	*left;
	char		*str;

	str = "has taken a dongle";
	left = coder->left_dongle;
	left->available = 0;
	locked_printf(coder->global_ptr, 0, coder->id, str);
	right = coder->right_dongle;
	right->available = 0;
	locked_printf(coder->global_ptr, 0, coder->id, str);
}

void	*worker(void *coder_ptr)
{
	t_coder		*coder;
	t_global	*g_data;

	coder = (t_coder *)coder_ptr;
	if (coder->left_dongle == coder->right_dongle)
		return (NULL);
	g_data = coder->global_ptr;
	wait_for_start(g_data);
	while (mutex_read_int(&g_data->lock_state, &g_data->state))
	{
		mutex_lock_worked(coder);
		if (!coder->in_queue)
			if (dongle_available(coder->left_dongle, coder->right_dongle))
				add_coder(coder);
		if (g_data->queue.head != coder)
		{
			mutex_unlock_worked(coder);
			continue ;
		}
		take_dongles(coder);
		queue_rm_head(coder);
		mutex_unlock_worked(coder);
		execute_coder(coder);
	}
	return (NULL);
}

int	monitoring_coder(t_coder *coder_tab, int *nb_coder_ended)
{
	t_global	*g_data;
	int			i;

	i = 0;
	g_data = coder_tab->global_ptr;
	while (i < g_data->args.number_of_coders)
	{
		g_data = coder_tab[i].global_ptr;
		if (mutex_read_burnout_time(coder_tab + i) <= get_time_ms())
		{
			locked_printf(g_data, 1, coder_tab[i].id, "burned out");
			return (0);
		}
		if (mutex_read_int(&coder_tab[i].lock, &((coder_tab + i)->nb_compil)) >= \
g_data->args.number_of_compiles_required)
			*nb_coder_ended = *nb_coder_ended + 1;
		i++;
	}
	return (1);
}

void	*monitoring(void *data)
{
	t_coder		*coder_tab;
	t_global	*g_data;
	int			state;
	int			nb_coder_ended;

	coder_tab = (t_coder *)data;
	g_data = coder_tab->global_ptr;
	state = 1;
	nb_coder_ended = 0;
	set_timestamp(coder_tab, g_data);
	mutex_write_int(&g_data->lock_state, &(g_data->state), 1);
	while (state && nb_coder_ended < g_data->args.number_of_coders)
	{
		nb_coder_ended = 0;
		state = monitoring_coder(coder_tab, &nb_coder_ended);
		usleep(100);
	}
	mutex_write_int(&g_data->lock_state, &(g_data->state), 0);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;	
	t_global	g_data;
	t_arg		args;
	int			i;

	if ((ac != 9) || (!parsing_arg(av + 1, &args)))
		return (exit_error_parsing());
	g_data = init_g_data(args);
	dongle_tab = init_dongle_tab(args.number_of_coders);
	coder_tab = init_coder_tab(args.number_of_coders, dongle_tab, &g_data);
	if ((!coder_tab) || (!dongle_tab) || (!g_data.threads))
		return (error_exit_free(coder_tab, dongle_tab, g_data));
	i = -1;
	set_variable(coder_tab, dongle_tab);
	while (++i < args.number_of_coders)
		pthread_create(&g_data.threads[i], NULL, worker, coder_tab + i);
	pthread_create(&g_data.threads[i], NULL, monitoring, coder_tab);
	i = -1;
	while (++i < args.number_of_coders + 1)
		pthread_join(g_data.threads[i], NULL);
	return (exit_free(coder_tab, dongle_tab, g_data));
}
