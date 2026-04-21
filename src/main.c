/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/21 11:22:24 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*worker(void *coder_ptr)
{
	t_coder		*coder;
	t_global	*g_data;

	coder = (t_coder *)coder_ptr;
	if (coder->left_dongle == coder->right_dongle)
		return (NULL);
	g_data = coder->global_ptr;
	while (mutex_read_int(&g_data->lock_state, &g_data->state))
	{
		if (coder->in_queue == 0)
			add_coder(coder);
		else
		{
			pthread_mutex_lock(&g_data->queue.lock);
			if (g_data->queue.head == coder)
			{
				rm_coder(coder);
				mutex_lock_dongle(coder);
				execute_coder(coder);
			}
			else
				pthread_mutex_unlock(&g_data->queue.lock);
		}
	}
	return (NULL);
}

int	monitoring_coder(t_coder *coder, int *nb_coder_ended)
{
	t_global	*g_data;

	g_data = coder->global_ptr;
	if (mutex_read_burnout_time(coder) < get_time_ms())
	{
		locked_printf(&g_data->lock_printf, "%ld %d burned out\n", \
get_time_since_start(g_data->start_time), coder->id);
		return (0);
	}
	if (mutex_read_int(&coder->lock, &(coder->nb_compil)) >= \
g_data->args.number_of_compiles_required)
		*nb_coder_ended = *nb_coder_ended + 1;
	return (1);
}

void	*monitoring(void *data)
{
	t_coder		*coder_tab;
	t_global	*g_data;
	int			state;
	int			nb_coder_ended;
	int			i;

	coder_tab = (t_coder *)data;
	g_data = coder_tab[0].global_ptr;
	state = 1;
	nb_coder_ended = 0;
	while (state && nb_coder_ended < g_data->args.number_of_coders)
	{
		i = -1;
		nb_coder_ended = 0;
		while (++i < g_data->args.number_of_coders && state)
			state = monitoring_coder(coder_tab + i, &nb_coder_ended);
		if (state == 0)
			mutex_write_int(&g_data->lock_state, &(g_data->state), 0);
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
		return (exit_free(coder_tab, dongle_tab, g_data));
	i = -1;
	while (++i < args.number_of_coders)
		pthread_create(&g_data.threads[i], NULL, worker, coder_tab + i);
	pthread_create(&g_data.threads[i], NULL, monitoring, coder_tab);
	i = -1;
	while (++i < args.number_of_coders + 1)
		pthread_join(g_data.threads[i], NULL);
	return (exit_free(coder_tab, dongle_tab, g_data));
}
