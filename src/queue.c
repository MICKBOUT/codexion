/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 10:06:12 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/21 15:29:39 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	queue_sorted(t_coder *node, t_coder *coder, int priority)
{
	long	node_burnout;
	long	coder_burnout;

	if (priority == 0)
		return (1);
	node_burnout = mutex_read_burnout_time(node);
	coder_burnout = mutex_read_burnout_time(coder);
	return (node_burnout < coder_burnout);
}

void	queue_rm_head(t_coder *coder)
{
	t_coder	*left;
	t_coder	*right;
	t_coder	*node;
	t_coder	*tmp;

	node = coder;
	left = coder->left_dongle->left;
	right = coder->right_dongle->right;
	while (node->next)
	{
		if (node->next == left || node->next == right)
		{
			tmp = node->next;
			node->next = node->next->next;
			tmp->next = NULL;
			tmp->in_queue = 0;
		}
		else
			node = node->next;
	}
	coder->global_ptr->queue.head = coder->next;
	coder->in_queue = 0;
}

// priority = 0: fifo
// priority = 1: edf
void	add_coder(t_coder *coder)
{
	t_global	*g_data;
	t_queue		*queue;
	t_coder		*node;
	int			priority;

	g_data = coder->global_ptr;
	queue = &g_data->queue;
	priority = (g_data->args.scheduler[0] == 'e');
	if ((!queue->head) || (!queue_sorted(queue->head, coder, priority)))
	{
		coder->next = queue->head;
		queue->head = coder;
	}
	else
	{
		node = queue->head;
		while (node->next && (queue_sorted(node->next, coder, priority)))
			node = node->next;
		coder->next = node->next;
		node->next = coder;
	}
	coder->in_queue = 1;
}
