/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader_test.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:47:43 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/25 19:18:19 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_map_reader	r;
	t_map_entry		e;

	if (argc != 2)
		return (1);
	if (!map_reader_open(&r, argv[1]))
		return (perror("open"), 2);
	while (1)
	{
		if (!map_reader_next_entry(&r, &e))
			return (perror("next"), 3);
		if (e.is_end)
			return (printf("done\n"), 0);
		if (e.is_newline)
			printf("nl\n");
		else
			printf("z:%d, col:%08x\n", e.z, e.color);
	}
}
