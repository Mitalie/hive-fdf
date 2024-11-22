/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_builder_add_entry.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:13:19 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 19:15:24 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

static bool	map_builder_append_vertex(
	t_map_builder *builder, int32_t z, uint32_t color)
{
	t_vertex	vertex;

	vertex.pos.x = builder->x;
	vertex.pos.y = builder->y;
	vertex.pos.z = z;
	vertex.pos.w = 1;
	vertex.color = color;
	builder->n_vertices++;
	return (buffer_append(&builder->vertices, &vertex, sizeof vertex));
}

static bool	map_builder_append_line(t_map_builder *builder, uint32_t offset)
{
	t_line		line;

	line.a = builder->n_vertices - 1;
	line.b = builder->n_vertices - 1 - offset;
	builder->n_lines++;
	return (buffer_append(&builder->lines, &line, sizeof line));
}

bool	map_builder_add_entry(t_map_builder *builder, t_map_entry *entry)
{
	if (entry->is_newline)
	{
		builder->last_x = builder->x;
		builder->x = 0;
		builder->y++;
		return (true);
	}
	if (!map_builder_append_vertex(builder, entry->z, entry->color))
		return (false);
	if (builder->x > 0)
		if (!map_builder_append_line(builder, 1))
			return (false);
	if (builder->x < builder->last_x)
		if (!map_builder_append_line(builder, builder->last_x))
			return (false);
	builder->x++;
	return (true);
}
