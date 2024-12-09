/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:29:17 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 20:33:31 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mesh.h"
#include "line.h"

void	draw_mesh(t_z_image *image, t_mesh *mesh, t_mat4 *transform)
{
	uint32_t	i;
	t_line		line;
	t_vertex	a;
	t_vertex	b;

	i = 0;
	while (i < mesh->n_lines)
	{
		line = mesh->lines[i++];
		a = mesh->vertices[line.a];
		a.pos = mul_mv4(transform, a.pos);
		b = mesh->vertices[line.b];
		b.pos = mul_mv4(transform, b.pos);
		draw_line(image, a, b);
	}
}
