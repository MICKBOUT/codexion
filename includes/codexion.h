/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:09 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/15 15:28:45 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_THREADS 4
#define INCREMENTS 1000000

typedef struct s_dongle
{
	pthread_mutex_t	lock_dispo;
	int				available;
	int				number;
}	t_dongle;

typedef struct s_coder
{
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	int			number;
}	t_coder;

typedef struct s_worker_data
{
	int			time_to_compile;
	t_coder		*coder_ptr;
	
}	t_worker_data;

typedef struct s_arg
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_arg;

int			parsing_arg(char **av, t_arg *arg);

t_coder		*init_coder_tab(int nb_coders, t_dongle *dongle_tab);
t_dongle	*init_dongle_tab(int nb_coders);

int			exit_error_parsing(void);
int			exit_free_ptr(void *ptr1, void *ptr2);