/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:39:26 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/17 17:06:52 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdbool.h>
# include "MLX42/MLX42.h"
# include "z_image.h"
# include "mesh.h"
# include "camera.h"

typedef struct s_fdf {
	mlx_t		*mlx;
	t_z_image	*image;
	t_mesh		mesh;
	t_camera	camera;
	bool		need_redraw;
	float		height_scale_exp;
}	t_fdf;

void	fdf_draw(t_fdf *fdf);

#endif
