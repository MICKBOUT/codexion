/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 10:06:12 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/27 16:09:20 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	queue_sorted(t_coder *node, t_coder *coder, int priority)
{
	if (!priority)
		return (1);
	return (mutex_read_burnout_time(node) <= mutex_read_burnout_time(coder));
}

void	queue_rm_head(t_coder *coder)
{
	coder->global_ptr->queue.head = coder->next;
	coder->in_queue = 0;
}

// priority = 0: fifo
// priority = 1: edf
void	add_coder(t_coder *coder)
{
	t_global	*g_data;
	t_queue		*queue;
	t_coder		**current;
	int			priority;

	g_data = coder->global_ptr;
	queue = &g_data->queue;
	priority = g_data->args.scheduler[0] == 'e';
	current = &queue->head;
	while (*current && queue_sorted(*current, coder, priority))
		current = &(*current)->next;
	coder->next = *current;
	*current = coder;
	coder->in_queue = 1;
}
