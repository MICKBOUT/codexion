/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 13:48:33 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*worker(void *coder_ptr)
{
	t_coder		*coder;
	t_global	*g_data;
	int			i;

	coder = (t_coder *)coder_ptr;
	g_data = coder->global_ptr;
	i = 0;
	while (i < g_data->args.number_of_compiles_required && g_data->status)
	{
		mutex_lock_dongle_coder(*coder);
		while (dongle_unavailable(coder->left_dongle, coder->right_dongle) || \
coder->left_dongle == coder->right_dongle)
		{
			pthread_mutex_unlock(&coder->left_dongle->lock_available);
			pthread_mutex_unlock(&coder->right_dongle->lock_available);
			if (!g_data->status)
				return (NULL);
			mutex_lock_dongle_coder(*coder);
		}
		execute_coder(coder);
		i++;
	}
	coder->running = 0;
	return (NULL);
}

int	monitoring_coder(t_coder *coder)
{
	t_global	*g_data;

	g_data = coder->global_ptr;
	if ((coder->burnout_time) < get_time_ms())
	{
		g_data->status = 0;
		locked_printf(&g_data->lock_printf, "%ld %d burned out\n", \
get_time_since_start(g_data->start_time), coder->number);
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
	while (running == 1)
	{
		running = 0;
		i = -1;
		while (++i < g_data->args.number_of_coders && running != -1)
			if (coder_tab[i].running)
				running = monitoring_coder(coder_tab + i);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;	
	t_global	global_data;
	t_arg		args;
	int			i;

	if ((ac != 9) || (!parsing_arg(av + 1, &args)))
		return (exit_error_parsing());
	global_data = init_global_data(args);
	dongle_tab = init_dongle_tab(args.number_of_coders);
	coder_tab = init_coder_tab(args.number_of_coders, dongle_tab, &global_data);
	if ((!coder_tab) || (!dongle_tab) || (!global_data.threads))
		return (exit_free(coder_tab, dongle_tab, global_data));
	i = -1;
	while (++i < args.number_of_coders)
		pthread_create(&global_data.threads[i], NULL, worker, coder_tab + i);
	pthread_create(&global_data.threads[i], NULL, monitoring, coder_tab);
	i = -1;
	while (++i < args.number_of_coders + 1)
		pthread_join(global_data.threads[i], NULL);
	return (exit_free(coder_tab, dongle_tab, global_data));
}
