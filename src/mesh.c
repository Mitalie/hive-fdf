/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:29:17 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/13 18:42:58 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mesh.h"
#include "line.h"

void	draw_mesh(t_z_image *image, t_mesh *mesh, t_mat4 *transform,
	float height_scale)
{
	uint32_t	i;
	t_line		line;
	t_vertex	a;
	t_vertex	b;
	t_mat4		mesh_transform;

	mesh_transform = mat4(
			vec4(1, 0, 0, mesh->pos.x),
			vec4(0, height_scale, 0, mesh->pos.y),
			vec4(0, 0, 1, mesh->pos.z),
			vec4(0, 0, 0, 1));
	mesh_transform = mul_mm4(transform, &mesh_transform);
	i = 0;
	while (i < mesh->n_lines)
	{
		line = mesh->lines[i++];
		a = mesh->vertices[line.a];
		a.pos = mul_mv4(&mesh_transform, a.pos);
		b = mesh->vertices[line.b];
		b.pos = mul_mv4(&mesh_transform, b.pos);
		draw_line(image, a, b);
	}
}
