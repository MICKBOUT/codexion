/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:09 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/13 15:15:32 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define NUM_THREADS 4
#define INCREMENTS 1000000

typedef struct s_data
{
	pthread_mutex_t	lock;
	int				counter;
}	t_data;

typedef struct s_coder
{
	int	number;
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
