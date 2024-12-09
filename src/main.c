/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 17:54:43 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "MLX42/MLX42.h"
#include "camera.h"
#include "line.h"
#include "mesh.h"
#include "map.h"

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_mesh		mesh;
	t_camera	camera;
	bool		need_redraw;
}	t_data;

static void	clear_image(mlx_image_t *image)
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
			mlx_put_pixel(image, x++, y, 0x000000ff);
		y++;
	}
}

static void	draw_with_camera(mlx_image_t *image, t_mesh *mesh, t_camera *camera)
{
	t_mat4	transform;

	transform = camera_transformation(camera);
	clear_image(image);
	draw_mesh(image, mesh, &transform);
}

static void	key_hook(mlx_key_data_t key_data, void *param)
{
	t_data	*fdf_data;

	fdf_data = param;
	if (key_data.key == MLX_KEY_ESCAPE && key_data.action == MLX_PRESS)
	{
		mlx_close_window(fdf_data->mlx);
		return ;
	}
	else if (key_data.key == MLX_KEY_R && key_data.action == MLX_PRESS)
		camera_reset(&fdf_data->camera);
	else if (key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
	{
		if (key_data.key == MLX_KEY_LEFT)
			fdf_data->camera.azimuth_deg += 5;
		else if (key_data.key == MLX_KEY_RIGHT)
			fdf_data->camera.azimuth_deg -= 5;
		else if (key_data.key == MLX_KEY_UP)
			fdf_data->camera.elevation_deg += 2;
		else if (key_data.key == MLX_KEY_DOWN)
			fdf_data->camera.elevation_deg -= 2;
		else if (key_data.key == MLX_KEY_W)
			camera_move(&fdf_data->camera, 0, 0, -1);
		else if (key_data.key == MLX_KEY_S)
			camera_move(&fdf_data->camera, 0, 0, 1);
		else if (key_data.key == MLX_KEY_A)
			camera_move(&fdf_data->camera, -1, 0, 0);
		else if (key_data.key == MLX_KEY_D)
			camera_move(&fdf_data->camera, 1, 0, 0);
		else if (key_data.key == MLX_KEY_Q)
			camera_move(&fdf_data->camera, 0, 1, 0);
		else if (key_data.key == MLX_KEY_Z)
			camera_move(&fdf_data->camera, 0, -1, 0);
	}
	fdf_data->need_redraw = true;
}

static void	loop_hook(void *param)
{
	t_data	*fdf_data;
	mlx_t	*mlx;

	fdf_data = param;
	mlx = fdf_data->mlx;
	if (fdf_data->image->width != (uint32_t)mlx->width
		|| fdf_data->image->height != (uint32_t)mlx->height)
	{
		mlx_delete_image(mlx, fdf_data->image);
		fdf_data->image = mlx_new_image(mlx, mlx->width, mlx->height);
		fdf_data->camera.aspect_ratio = (float)mlx->width / mlx->height;
		if (!fdf_data->image)
		{
			mlx_close_window(mlx);
			return ;
		}
		mlx_image_to_window(mlx, fdf_data->image, 0, 0);
		fdf_data->need_redraw = true;
	}
	if (fdf_data->need_redraw)
		draw_with_camera(fdf_data->image, &fdf_data->mesh, &fdf_data->camera);
	fdf_data->need_redraw = false;
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (3);
	if (!map_load(&data.mesh, argv[1]))
		return (2);
	data.mlx = mlx_init(1500, 1125, "fdf", true);
	if (!data.mlx)
		return (1);
	data.image = mlx_new_image(data.mlx, data.mlx->width, data.mlx->height);
	camera_reset(&data.camera);
	data.camera.aspect_ratio = (float)data.mlx->width / data.mlx->height;
	if (data.image)
	{
		mlx_key_hook(data.mlx, key_hook, &data);
		mlx_loop_hook(data.mlx, loop_hook, &data);
		data.need_redraw = true;
		mlx_image_to_window(data.mlx, data.image, 0, 0);
	}
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	free(data.mesh.lines);
	free(data.mesh.vertices);
}
