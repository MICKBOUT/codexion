/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 15:20:02 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_debug(t_coder coder)
{
	struct timeval	time;
	long			t_start;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(&coder.global_ptr->lock_printf);
	printf("%ld %d is debugging\n", get_time_since_start(coder.global_ptr), coder.number);
	pthread_mutex_unlock(&coder.global_ptr->lock_printf);
	t_start = time.tv_sec * 1000000 + time.tv_usec;
	while ((time.tv_sec * 1000000 + time.tv_usec) - t_start < (coder.global_ptr->time_to_debug * 1000))
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
}

static void	coder_compile(t_coder coder)
{
	struct timeval	time;
	long			t_start;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(&coder.global_ptr->lock_printf);
	printf("%ld %d is compiling\n", get_time_since_start(coder.global_ptr), coder.number);
	pthread_mutex_unlock(&coder.global_ptr->lock_printf);
	t_start = time.tv_sec * 1000000 + time.tv_usec;
	while ((time.tv_sec * 1000000 + time.tv_usec) - t_start < (coder.global_ptr->time_to_compile * 1000))
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
	set_bot_dongle_available(coder);
	pthread_mutex_lock(&coder.global_ptr->lock_printf);
	pthread_mutex_unlock(&coder.global_ptr->lock_printf);
}

static void	*execute_coder(t_coder coder)
{
	coder_compile(coder);
	coder_debug(coder);
	return (NULL);
}

void	*worker(void *coder_ptr)
{
	t_coder	coder;

	coder = *(t_coder *)coder_ptr;
	mutex_lock_dongle_coder(coder);
	while (coder.left_dongle->available == 0 || coder.right_dongle->available == 0)
	{
		pthread_mutex_unlock(&coder.left_dongle->lock_available);
		pthread_mutex_unlock(&coder.right_dongle->lock_available);
		mutex_lock_dongle_coder(coder);
	}
	coder.left_dongle->available = 0;
	coder.right_dongle->available = 0;
	pthread_mutex_lock(&coder.global_ptr->lock_printf);
	printf("%ld %d has taken a dongle\n", get_time_since_start(coder.global_ptr), coder.number);
	printf("%ld %d has taken a dongle\n", get_time_since_start(coder.global_ptr), coder.number);
	pthread_mutex_unlock(&coder.global_ptr->lock_printf);
	pthread_mutex_unlock(&coder.left_dongle->lock_available);
	pthread_mutex_unlock(&coder.right_dongle->lock_available);
	return (execute_coder(coder));
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
		return (exit_free_ptr(coder_tab, dongle_tab, global_data, args.number_of_coders));
	i = -1;
	while (++i < args.number_of_coders)
		pthread_create(&global_data.threads[i], NULL, worker, coder_tab + i);
	i = -1;
	while (++i < args.number_of_coders)
		pthread_join(global_data.threads[i], NULL);
	return (exit_free_ptr(coder_tab, dongle_tab, global_data, args.number_of_coders));
}
