/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:24:11 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/13 18:42:50 by amakinen         ###   ########.fr       */
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
	uint32_t	n_lines;
	t_vec4		pos;
}	t_mesh;

void	draw_mesh(t_z_image *image, t_mesh *mesh, t_mat4 *transform,
			float height_scale);

#endif
