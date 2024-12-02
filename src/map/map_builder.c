/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:12:53 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/02 17:59:01 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"
#include <stdlib.h>

void	map_builder_init(t_map_builder *builder)
{
	buffer_init(&builder->vertices);
	buffer_init(&builder->lines);
	builder->z = 0;
	builder->x = 0;
	builder->last_x = 0;
	builder->max_x = 0;
	builder->n_vertices = 0;
	builder->n_lines = 0;
}

void	map_builder_release(t_map_builder *builder)
{
	buffer_release(&builder->vertices);
	buffer_release(&builder->lines);
}

/*
	Center the model in the horizontal plane
*/
static void	map_builder_adjust_vertices(t_map_builder *builder, t_mesh *mesh)
{
	uint32_t	i;
	float		adj_x;
	float		adj_z;

	adj_x = (builder->max_x - 1) * -0.5f;
	adj_z = (builder->z - 1) * -0.5f;
	i = 0;
	while (i < builder->n_vertices)
	{
		mesh->vertices[i].pos.x += adj_x;
		mesh->vertices[i].pos.z += adj_z;
		i++;
	}
}

bool	map_builder_finalize(t_map_builder *builder, t_mesh *mesh)
{
	mesh->vertices = buffer_finalize(&builder->vertices);
	mesh->lines = buffer_finalize(&builder->lines);
	mesh->n_lines = builder->n_lines;
	map_builder_release(builder);
	if (!mesh->vertices || !mesh->lines)
	{
		free(mesh->vertices);
		free(mesh->lines);
		return (false);
	}
	map_builder_adjust_vertices(builder, mesh);
	return (true);
}
