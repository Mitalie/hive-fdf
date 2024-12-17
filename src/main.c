/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/17 15:41:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <math.h>
#include <stdlib.h>
#include "MLX42/MLX42.h"
#include "z_image.h"
#include "camera.h"
#include "line.h"
#include "mesh.h"
#include "map.h"
#include "input.h"

static void	fdf_draw(t_fdf *fdf)
{
	t_mat4	transform;
	float	height_scale;

	if (!fdf->need_redraw)
		return ;
	fdf->need_redraw = false;
	transform = camera_transformation(&fdf->camera);
	if (fdf->camera.perspective)
		z_image_clear(fdf->image, 0.0f, 0x000000ff);
	else
		z_image_clear(fdf->image, -INFINITY, 0x000000ff);
	height_scale = powf(2, fdf->height_scale_exp);
	fdf->mesh.scale.y = height_scale;
	draw_mesh(fdf->image, &fdf->mesh, transform);
}

static void	key_hook(mlx_key_data_t key_data, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	if (key_data.key == MLX_KEY_ESCAPE && key_data.action == MLX_PRESS)
	{
		mlx_close_window(fdf->mlx);
		return ;
	}
	else if (key_data.key == MLX_KEY_R && key_data.action == MLX_PRESS)
	{
		camera_reset(&fdf->camera);
		fdf->height_scale_exp = -2;
	}
	else if (key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
	{
		if (key_data.key == MLX_KEY_P)
			camera_toggle_mode(&fdf->camera);
		else if (key_data.key == MLX_KEY_M)
			fdf->camera.move_angled = !fdf->camera.move_angled;
		else if (key_data.key == MLX_KEY_KP_ADD)
			fdf->camera.zoom_exp += 0.5f;
		else if (key_data.key == MLX_KEY_KP_SUBTRACT)
			fdf->camera.zoom_exp -= 0.5f;
		else if (key_data.key == MLX_KEY_PAGE_UP)
			fdf->height_scale_exp += 0.5f;
		else if (key_data.key == MLX_KEY_PAGE_DOWN)
			fdf->height_scale_exp -= 0.5f;
	}
	fdf->need_redraw = true;
}

static void	loop_hook(void *param)
{
	t_fdf	*fdf;
	mlx_t	*mlx;

	fdf = param;
	mlx = fdf->mlx;
	if (fdf->image->mlx_img->width != (uint32_t)mlx->width
		|| fdf->image->mlx_img->height != (uint32_t)mlx->height)
	{
		z_image_delete(mlx, fdf->image);
		fdf->image = z_image_new(mlx, mlx->width, mlx->height);
		fdf->camera.aspect_ratio = (float)mlx->width / mlx->height;
		if (!fdf->image)
		{
			mlx_close_window(mlx);
			return ;
		}
		mlx_image_to_window(mlx, fdf->image->mlx_img, 0, 0);
		fdf->need_redraw = true;
	}
	input_timed(fdf);
	fdf_draw(fdf);
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;

	if (argc != 2)
		return (3);
	if (!map_load(&fdf.mesh, argv[1]))
		return (2);
	fdf.mlx = mlx_init(1500, 1125, "fdf", true);
	if (!fdf.mlx)
		return (1);
	fdf.image = z_image_new(fdf.mlx, fdf.mlx->width, fdf.mlx->height);
	camera_reset(&fdf.camera);
	fdf.camera.aspect_ratio = (float)fdf.mlx->width / fdf.mlx->height;
	fdf.height_scale_exp = -2;
	if (fdf.image)
	{
		mlx_key_hook(fdf.mlx, key_hook, &fdf);
		mlx_loop_hook(fdf.mlx, loop_hook, &fdf);
		fdf.need_redraw = true;
		mlx_image_to_window(fdf.mlx, fdf.image->mlx_img, 0, 0);
	}
	mlx_loop(fdf.mlx);
	z_image_delete(fdf.mlx, fdf.image);
	mlx_terminate(fdf.mlx);
	free(fdf.mesh.lines);
	free(fdf.mesh.vertices);
}
