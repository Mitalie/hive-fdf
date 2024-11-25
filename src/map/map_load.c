/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:10:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/25 19:15:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "map_internal.h"

bool	map_load(t_mesh *mesh, const char *filename)
{
	t_map_reader	reader;
	t_map_builder	builder;
	t_map_entry		entry;

	if (!map_reader_open(&reader, filename))
		return (false);
	map_builder_init(&builder);
	while (1)
	{
		if (!map_reader_next_entry(&reader, &entry))
			break ;
		if (!map_builder_add_entry(&builder, &entry))
			break ;
		if (entry.is_end)
		{
			map_reader_close(&reader);
			return (map_builder_finalize(&builder, mesh));
		}
	}
	map_reader_close(&reader);
	map_builder_release(&builder);
	return (false);
}
