/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:29:17 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/16 16:13:56 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mesh.h"
#include "line.h"

static t_mat4	mesh_combine_transform(t_mesh *mesh, t_mat4 *transform)
{
	t_mat4		mesh_transform;

	mesh_transform = mat4(
			vec4(mesh->scale.x, 0, 0, mesh->pos.x),
			vec4(0, mesh->scale.y, 0, mesh->pos.y),
			vec4(0, 0, mesh->scale.z, mesh->pos.z),
			vec4(0, 0, 0, 1));
	return (mul_mm4(transform, &mesh_transform));
}

void	draw_mesh(t_z_image *image, t_mesh *mesh, t_mat4 *transform)
{
	uint32_t	i;
	t_line		line;
	t_vertex	a;
	t_vertex	b;
	t_mat4		combined_transform;

	combined_transform = mesh_combine_transform(mesh, transform);
	i = 0;
	while (i < mesh->n_lines)
	{
		line = mesh->lines[i++];
		a = mesh->vertices[line.a];
		a.pos = mul_mv4(&combined_transform, a.pos);
		b = mesh->vertices[line.b];
		b.pos = mul_mv4(&combined_transform, b.pos);
		draw_line(image, a, b);
	}
}
