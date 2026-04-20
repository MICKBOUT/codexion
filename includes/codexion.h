/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:09 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/20 18:14:39 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

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

typedef struct s_dongle
{
	pthread_mutex_t	lock_available;
	long			end_cooldown;
	int				available;
	int				id;
}	t_dongle;

typedef struct s_global
{
	pthread_mutex_t	lock_state;
	pthread_mutex_t	lock_printf;
	pthread_t		*threads;
	t_arg			args;
	int				state;
	long			start_time;
}	t_global;

typedef struct s_coder
{
	pthread_mutex_t	lock;
	t_global		*global_ptr;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			burnout_time;
	int				nb_compil;
	int				running;
	int				id;
}	t_coder;

//execute_coder.c
void		execute_coder(t_coder *coder);

// exit.c
int			exit_error_parsing(void);
int			exit_free(\
t_coder *coder_tab, t_dongle *dongle_tab, t_global g_data);

// init.c
t_global	init_g_data(t_arg args);
t_dongle	*init_dongle_tab(int nb_coders);
t_coder		*init_coder_tab(\
int nb_coders, t_dongle *dongle_tab, t_global *g_data);

// parsing.c
int			parsing_arg(char **av, t_arg *arg);

//timing.c
long		get_time_ms(void);
long		get_time_since_start(long start_time);

// utils_mutex_lock.c
void		mutex_lock_dongle(t_coder *coder);
void		mutex_unlock_dongle(t_coder *coder);

// utils_mutex.c
void		mutex_add_int(pthread_mutex_t *lock, int *place, int add);
void		mutex_write_int(pthread_mutex_t *lock, int *place, int var);
int			mutex_read_int(pthread_mutex_t *lock, int *place);
long		mutex_read_burnout_time(t_coder *coder);
void		mutex_write_burnout_time(t_coder *coder, long time);

// utils.c
void		set_dongle_available_after_cooldown(t_coder *coder);
void		locked_printf(pthread_mutex_t *lock, const char *fmt, ...);
int			dongle_available(t_dongle *l_dongle, t_dongle *r_dongle);