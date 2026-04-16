/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 17:58:06 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*worker(void *coder_ptr)
{
	t_coder		coder;

	coder = *(t_coder *)coder_ptr;
	mutex_lock_dongle_coder(coder);
	while (dongle_available(coder.left_dongle, coder.right_dongle) == 0)
	{
		pthread_mutex_unlock(&coder.left_dongle->lock_available);
		pthread_mutex_unlock(&coder.right_dongle->lock_available);
		mutex_lock_dongle_coder(coder);
	}
	execute_coder(coder);
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
	i = -1;
	while (++i < args.number_of_coders)
		pthread_join(global_data.threads[i], NULL);
	return (exit_free(coder_tab, dongle_tab, global_data));
}
