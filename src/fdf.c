/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:02:27 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:47:38 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "camera.h"
#include "mesh.h"
#include "vec_mat.h"
#include "z_image.h"

/*
	Create or recreate image. Does nothing if image exists and matches current
	window size.
*/
void	fdf_recreate_image(t_fdf *fdf)
{
	mlx_t	*mlx;

	mlx = fdf->mlx;
	if (fdf->image
		&& fdf->image->mlx_img->height == (uint32_t)mlx->height
		&& fdf->image->mlx_img->width == (uint32_t)mlx->width
	)
		return ;
	if (fdf->image)
		z_image_delete(fdf->mlx, fdf->image);
	fdf->image = z_image_new(mlx, mlx->width, mlx->height);
	if (!fdf->image)
	{
		mlx_close_window(mlx);
		return ;
	}
	mlx_image_to_window(mlx, fdf->image->mlx_img, 0, 0);
	fdf->camera.aspect_ratio = (float)mlx->width / mlx->height;
	fdf->need_redraw = true;
}

void	fdf_reset(t_fdf *fdf)
{
	camera_reset(&fdf->camera);
	fdf->height_scale_exp = -2;
	fdf_fit(fdf);
}

void	fdf_fit(t_fdf *fdf)
{
	t_mat4	camera_transform;
	float	height_scale;
	t_vec4	box_min;
	t_vec4	box_max;

	if (fdf->camera.mode != CAM_ORTHO)
		return ;
	camera_transform = camera_transformation(&fdf->camera);
	height_scale = powf(2, fdf->height_scale_exp);
	fdf->mesh.scale.y = height_scale;
	mesh_calculate_box(&fdf->mesh, camera_transform, &box_min, &box_max);
	camera_fit_box(&fdf->camera, box_min, box_max);
}

void	fdf_draw(t_fdf *fdf)
{
	t_mat4	transform;
	float	height_scale;

	if (!fdf->need_redraw)
		return ;
	fdf->need_redraw = false;
	transform = camera_transformation(&fdf->camera);
	if (fdf->camera.mode == CAM_ORTHO)
		z_image_clear(fdf->image, -INFINITY, 0x000000ff);
	else
		z_image_clear(fdf->image, 0.0f, 0x000000ff);
	height_scale = powf(2, fdf->height_scale_exp);
	fdf->mesh.scale.y = height_scale;
	draw_mesh(fdf->image, &fdf->mesh, transform);
}
