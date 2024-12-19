/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:12:53 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:48:09 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"
#include "mesh.h"
#include "vec_mat.h"

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
static void	map_builder_set_position(t_map_builder *builder, t_mesh *mesh)
{
	float		adj_x;
	float		adj_z;

	adj_x = -0.5f * (builder->max_x - 1);
	adj_z = -0.5f * (builder->z - 1);
	mesh->pos = vec4(adj_x, 0, adj_z, 1);
}

bool	map_builder_finalize(t_map_builder *builder, t_mesh *mesh)
{
	mesh->vertices = buffer_finalize(&builder->vertices);
	mesh->n_vertices = builder->n_vertices;
	mesh->lines = buffer_finalize(&builder->lines);
	mesh->n_lines = builder->n_lines;
	map_builder_release(builder);
	if (!mesh->vertices || !mesh->lines)
	{
		free(mesh->vertices);
		free(mesh->lines);
		return (false);
	}
	map_builder_set_position(builder, mesh);
	mesh->scale = vec4(1, 1, 1, 1);
	return (true);
}
