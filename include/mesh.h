/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:24:11 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/16 22:07:34 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_H
# define MESH_H

# include <stdint.h>
# include "z_image.h"
# include "vec_mat.h"
# include "vertex.h"

typedef struct s_line
{
	uint32_t	a;
	uint32_t	b;
}	t_line;

typedef struct s_mesh
{
	t_vertex	*vertices;
	t_line		*lines;
	uint32_t	n_vertices;
	uint32_t	n_lines;
	t_vec4		pos;
	t_vec4		scale;
}	t_mesh;

/*
	Position and scale should really be properties of a mesh instance, with
	multiple instances able to reference the same mesh. We only display one mesh
	at a time though, so skip the extra layer of indirection for now.
*/

void	draw_mesh(t_z_image *image, t_mesh *mesh, t_mat4 transform);
void	mesh_calculate_box(t_mesh *mesh, t_mat4 transform,
			t_vec4 *box_min, t_vec4 *box_max);

#endif
