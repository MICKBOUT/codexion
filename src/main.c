/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 16:58:57 by mboutte          ###   ########.fr       */
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
	while (coder->nb_compil < g_data->args.number_of_compiles_required)
	{
		mutex_lock_dongle(*coder);
		while (dongle_unavailable(coder->left_dongle, coder->right_dongle))
		{
			pthread_mutex_unlock(&coder->left_dongle->lock_available);
			pthread_mutex_unlock(&coder->right_dongle->lock_available);
			if (!mutex_read_int(&g_data->lock_state, &g_data->state))
				return (NULL);
			mutex_lock_dongle(*coder);
		}
		execute_coder(coder);
		coder->nb_compil++;
	}
	mutex_write_int(&coder->lock, &(coder->running), 0);
	return (NULL);
}

int	monitoring_coder(t_coder *coder)
{
	t_global	*g_data;

	g_data = coder->global_ptr;
	if (mutex_read_burnout_time(coder) < get_time_ms())
	{
		pthread_mutex_lock(&g_data->lock_state);
		g_data->state = 0;
		pthread_mutex_unlock(&g_data->lock_state);
		locked_printf(&g_data->lock_printf, "%ld %d burned out\n", \
get_time_since_start(g_data->start_time), coder->id);
		return (-1);
	}
	return (1);
}

void	*monitoring(void *data)
{
	t_coder		*coder_tab;
	t_global	*g_data;
	int			running;
	int			i;

	coder_tab = (t_coder *)data;
	g_data = coder_tab[0].global_ptr;
	running = 1;
	while (running > 0)
	{
		running = 0;
		i = -1;
		while (++i < g_data->args.number_of_coders && running != -1)
			if (mutex_read_int(&coder_tab[i].lock, &(coder_tab[i].running)))
				running = monitoring_coder(coder_tab + i);
		usleep(100);
	}
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
