/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:12:53 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 18:15:51 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"
#include <stdlib.h>

void	map_builder_init(t_map_builder *builder)
{
	buffer_init(&builder->vertices);
	buffer_init(&builder->lines);
	builder->y = 0;
	builder->x = 0;
	builder->last_x = 0;
	builder->n_vertices = 0;
	builder->n_lines = 0;
}

void	map_builder_release(t_map_builder *builder)
{
	buffer_release(&builder->vertices);
	buffer_release(&builder->lines);
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
	return (true);
}
