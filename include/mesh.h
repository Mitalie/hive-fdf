/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:24:11 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/21 18:30:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_H
# define MESH_H

# include <stdint.h>
# include "MLX42/MLX42.h"
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
}	t_mesh;

void	draw_mesh(mlx_image_t *image, t_mesh *mesh, t_mat4f *transform);

#endif
