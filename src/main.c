/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutte <mboutte@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:14 by mboutte           #+#    #+#             */
/*   Updated: 2026/04/14 18:01:54 by mboutte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_coder		*coder_tab;
	t_dongle	*dongle_tab;
	t_arg		arg;
	struct timeval tv;

	if ((ac != 9) || (!parsing_arg(av + 1, &arg)))
		return (exit_error_parsing());
	dongle_tab = init_dongle_tab(arg.number_of_coders);
	coder_tab = init_coder_tab(arg.number_of_coders, dongle_tab);
	if ((!coder_tab) || (!dongle_tab))
		return (exit_free_ptr(coder_tab, dongle_tab));

	gettimeofday(&tv, NULL);
	printf("Seconds: %ld\n", tv.tv_sec);
	printf("Microseconds: %ld\n", tv.tv_usec);
	
	for (int j = 0; j < arg.number_of_coders; j++)
		printf("coder_nb:%d|left_pt:%p|left_nb:%d|right_pt:%p|right_nb:%d|\n", coder_tab[j].number, coder_tab[j].left_dongle, coder_tab[j].left_dongle->number, coder_tab[j].right_dongle, coder_tab[j].right_dongle->number);
	return (exit_free_ptr(coder_tab, dongle_tab));
}
