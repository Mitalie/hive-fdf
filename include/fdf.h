/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:39:26 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:51:33 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdbool.h>
# include "MLX42/MLX42.h"

# include "camera.h"
# include "mesh.h"
# include "z_image.h"

typedef struct s_fdf {
	mlx_t		*mlx;
	t_z_image	*image;
	t_mesh		mesh;
	t_camera	camera;
	bool		need_redraw;
	float		height_scale_exp;
}	t_fdf;

void	fdf_recreate_image(t_fdf *fdf);
void	fdf_reset(t_fdf *fdf);
void	fdf_fit(t_fdf *fdf);
void	fdf_draw(t_fdf *fdf);

#endif
