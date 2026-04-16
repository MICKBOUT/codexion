/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:09 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/16 15:13:31 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_global
{
	pthread_mutex_t	lock_printf;
	pthread_t		*threads;
	int				time_to_compile;
	int				time_to_debug;
	long			start_time;
}	t_global;

typedef struct s_dongle
{
	pthread_mutex_t	lock_available;
	int				available;
	int				number;
}	t_dongle;

typedef struct s_coder
{
	t_global	*global_ptr;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	int			number;
}	t_coder;

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

// parsing.c
int			parsing_arg(char **av, t_arg *arg);

// init.c
t_global	init_global_data(t_arg args);
t_dongle	*init_dongle_tab(int nb_coders);
t_coder		*init_coder_tab(int nb_coders, t_dongle *dongle_tab, t_global *global_ptr);

// utils.c
void		set_bot_dongle_available(t_coder coder);
void		mutex_lock_dongle_coder(t_coder coder);
long		get_time_since_start(t_global *global_data);

// exit.c
int			exit_error_parsing(void);
int			exit_free_ptr(t_coder *coder_tab, t_dongle *dongle_tab, t_global global_data, int n);
