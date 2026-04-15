/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/15 16:38:18 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	worker_logic(t_coder coder)
{
	struct	timeval time;
	long	t_start;

	coder.left_dongle->available = 0;
	coder.right_dongle->available = 0;
	pthread_mutex_unlock(&coder.left_dongle->lock_dispo);
	pthread_mutex_unlock(&coder.right_dongle->lock_dispo);
	gettimeofday(&time, NULL);
	t_start = time.tv_sec * 1000000 + time.tv_usec;
	while ((time.tv_sec * 1000000 + time.tv_usec) - t_start < 10000)
	{
		usleep(1000);
		gettimeofday(&time, NULL);
	}
	pthread_mutex_lock(&coder.left_dongle->lock_dispo);
	coder.left_dongle->available = 1;
	pthread_mutex_unlock(&coder.left_dongle->lock_dispo);
	pthread_mutex_lock(&coder.right_dongle->lock_dispo);
	coder.right_dongle->available = 1;
	pthread_mutex_unlock(&coder.right_dongle->lock_dispo);
	// to do: mutex on printf
	printf("coder %d ended compiling, Dongle free in %ld us\n", coder.number, (time.tv_sec * 1000000 + time.tv_usec) - t_start);
	return ;
}

void	*worker(void *coder_ptr)
{
	t_coder	coder = *(t_coder *)coder_ptr;

	while (1)
	{	
		if (coder.left_dongle->number < coder.right_dongle->number)
			pthread_mutex_lock(&coder.left_dongle->lock_dispo);
		pthread_mutex_lock(&coder.right_dongle->lock_dispo);
		if (coder.left_dongle->number > coder.right_dongle->number)
			pthread_mutex_lock(&coder.left_dongle->lock_dispo);
		if (coder.left_dongle->available == 1 && coder.right_dongle->available == 1)
		{
			worker_logic(coder);
			return (NULL);
		}
		pthread_mutex_unlock(&coder.left_dongle->lock_dispo);
		pthread_mutex_unlock(&coder.right_dongle->lock_dispo);
	}
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;
	pthread_t	*threads;
	t_arg		args;

	if ((ac != 9) || (!parsing_arg(av + 1, &args)))
		return (exit_error_parsing());
	threads = malloc(sizeof(pthread_t) * args.number_of_coders);
	dongle_tab = init_dongle_tab(args.number_of_coders);
	coder_tab = init_coder_tab(args.number_of_coders, dongle_tab);
	if ((!coder_tab) || (!dongle_tab) || (!threads))
		return (exit_free_ptr(coder_tab, dongle_tab, threads, args.number_of_coders));
	for (int i = 0; i < args.number_of_coders; i++)
		printf("coder_nb:%d|left_pt:%p|left_nb:%d|right_pt:%p|right_nb:%d|\n", coder_tab[i].number, coder_tab[i].left_dongle, coder_tab[i].left_dongle->number, coder_tab[i].right_dongle, coder_tab[i].right_dongle->number);

	for (int i = 0; i < args.number_of_coders; i++)
		pthread_create(&threads[i], NULL, worker, coder_tab + i);

	for (int i = 0; i < args.number_of_coders; i++)
		pthread_join(threads[i], NULL);
	return (exit_free_ptr(coder_tab, dongle_tab, threads, args.number_of_coders));
}

