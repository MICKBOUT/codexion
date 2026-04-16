/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 14:36:00 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*worker_logic(t_coder coder)
{
	struct timeval	time;
	long			t_start;

	gettimeofday(&time, NULL);
	t_start = time.tv_sec * 1000000 + time.tv_usec;
	while ((time.tv_sec * 1000000 + time.tv_usec) - t_start < (coder.global_ptr->time_to_compile * 1000))
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
	set_bot_dongle_available(coder);
	pthread_mutex_lock(&coder.global_ptr->lock_printf);
	printf("coder %d ended compiling, Dongle free in %ld ms\n", coder.number, ((time.tv_sec * 1000000 + time.tv_usec) - t_start) / 1000);
	pthread_mutex_unlock(&coder.global_ptr->lock_printf);
	return (NULL);
}

void	*worker(void *coder_ptr)
{
	t_coder	coder;

	coder = *(t_coder *)coder_ptr;
	if (coder.left_dongle->number < coder.right_dongle->number)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
	pthread_mutex_lock(&coder.right_dongle->lock_available);
	if (coder.left_dongle->number > coder.right_dongle->number)
		pthread_mutex_lock(&coder.left_dongle->lock_available);
	while (coder.left_dongle->available == 0 || coder.right_dongle->available == 0)
	{	
		pthread_mutex_unlock(&coder.left_dongle->lock_available);
		pthread_mutex_unlock(&coder.right_dongle->lock_available);
		if (coder.left_dongle->number < coder.right_dongle->number)
			pthread_mutex_lock(&coder.left_dongle->lock_available);
		pthread_mutex_lock(&coder.right_dongle->lock_available);
		if (coder.left_dongle->number > coder.right_dongle->number)
			pthread_mutex_lock(&coder.left_dongle->lock_available);
	}
	coder.left_dongle->available = 0;
	pthread_mutex_unlock(&coder.left_dongle->lock_available);
	coder.right_dongle->available = 0;
	pthread_mutex_unlock(&coder.right_dongle->lock_available);
	return (worker_logic(coder));
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;
	t_global	global_data;
	t_arg		args;

	if ((ac != 9) || (!parsing_arg(av + 1, &args)))
		return (exit_error_parsing());
	global_data = init_global_data(args.time_to_compile, args.number_of_coders);
	dongle_tab = init_dongle_tab(args.number_of_coders);
	coder_tab = init_coder_tab(args.number_of_coders, dongle_tab, &global_data);
	if ((!coder_tab) || (!dongle_tab) || (!global_data.threads))
		return (exit_free_ptr(coder_tab, dongle_tab, global_data, args.number_of_coders));
	for (int i = 0; i < args.number_of_coders; i++)
		pthread_create(&global_data.threads[i], NULL, worker, coder_tab + i);
	for (int i = 0; i < args.number_of_coders; i++)
		pthread_join(global_data.threads[i], NULL);
	return (exit_free_ptr(coder_tab, dongle_tab, global_data, args.number_of_coders));
}
