/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/15 15:39:06 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*worker(void *coder_ptr)
{
	struct	timeval time;
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
			coder.left_dongle->available = 0;
			coder.right_dongle->available = 0;
			pthread_mutex_unlock(&coder.left_dongle->lock_dispo);
			pthread_mutex_unlock(&coder.right_dongle->lock_dispo);
			gettimeofday(&time, NULL);
			long t_start = time.tv_sec * 1000000 + time.tv_usec;
			while (1)
			{
				usleep(1000); //wait a bit
				gettimeofday(&time, NULL);
				if ((time.tv_sec * 1000000 + time.tv_usec) - t_start > 10000)
				{
					printf("coder %d ended compiling, Dongle free in %ld us\n", coder.number, (time.tv_sec * 1000000 + time.tv_usec) - t_start);
					coder.left_dongle->available = 1;
					coder.right_dongle->available = 1;
					return NULL;
				}
			}
		}
		pthread_mutex_unlock(&coder.left_dongle->lock_dispo);
		pthread_mutex_unlock(&coder.right_dongle->lock_dispo);
	}
}

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;
	pthread_t *threads;
	t_arg		args;

	if ((ac != 9) || (!parsing_arg(av + 1, &args)))
		return (exit_error_parsing());
	dongle_tab = init_dongle_tab(args.number_of_coders);
	coder_tab = init_coder_tab(args.number_of_coders, dongle_tab);
	if ((!coder_tab) || (!dongle_tab))
		return (exit_free_ptr(coder_tab, dongle_tab));
	for (int j = 0; j < args.number_of_coders; j++)
		printf("coder_nb:%d|left_pt:%p|left_nb:%d|right_pt:%p|right_nb:%d|\n", coder_tab[j].number, coder_tab[j].left_dongle, coder_tab[j].left_dongle->number, coder_tab[j].right_dongle, coder_tab[j].right_dongle->number);

	threads = malloc(sizeof(pthread_t) * args.number_of_coders);
	// Create threads
	for (int i = 0; i < args.number_of_coders; i++)
		pthread_create(&threads[i], NULL, worker, coder_tab + i);

	// Wait for threads
	for (int i = 0; i < args.number_of_coders; i++)
		pthread_join(threads[i], NULL);
	free(threads);
	return (exit_free_ptr(coder_tab, dongle_tab));
}

