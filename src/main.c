/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/05 21:56:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "MLX42/MLX42.h"
#include "line.h"
#include "mesh.h"
#include "map.h"

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_mesh		mesh;
	float		azimuth_deg;
	float		elevation_deg;
	t_vec4		camera_pos;
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

static void	move_camera(t_data *data, float right, float up, float back)
{
	t_mat4	transform;
	t_mat4	next;
	float	azimuth_rad;
	float	elevation_rad;

	// angles and order inverted to rotate camera vector in world instead of world around camera position
	elevation_rad = -data->elevation_deg / 180 * 3.1415926535;
	transform = mat4(
			vec4(1.0f, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, cosf(elevation_rad), -sinf(elevation_rad), 0.0f),
			vec4(0.0f, sinf(elevation_rad), cosf(elevation_rad), 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	azimuth_rad = -data->azimuth_deg / 180 * 3.1415926535;
	next = mat4(
			vec4(cosf(azimuth_rad), 0.0f, sinf(azimuth_rad), 0.0f),
			vec4(0.0f, 1.0f, 0.0f, 0.0f),
			vec4(-sinf(azimuth_rad), 0.0f, cosf(azimuth_rad), 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	transform = mul_mm4(&next, &transform);
	data->camera_pos = add4(data->camera_pos, mul_mv4(&transform, vec4(right, up, back, 1)));
}

static void	draw_with_angle(t_data *data)
{
	t_mat4	transform;
	t_mat4	next;
	float	azimuth_rad;
	float	elevation_rad;

	// move camera at (10,10,10) to origin
	transform = mat4(
			vec4(1.0f, 0.0f, 0.0f, -data->camera_pos.x),
			vec4(0.0f, 1.0f, 0.0f, -data->camera_pos.y),
			vec4(0.0f, 0.0f, 1.0f, -data->camera_pos.z),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// rotate camera in horizontal plane
	azimuth_rad = data->azimuth_deg / 180 * 3.1415926535;
	next = mat4(
			vec4(cosf(azimuth_rad), 0.0f, sinf(azimuth_rad), 0.0f),
			vec4(0.0f, 1.0f, 0.0f, 0.0f),
			vec4(-sinf(azimuth_rad), 0.0f, cosf(azimuth_rad), 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	transform = mul_mm4(&next, &transform);
	// rotate camera up/down
	elevation_rad = data->elevation_deg / 180 * 3.1415926535;
	next = mat4(
			vec4(1.0f, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, cosf(elevation_rad), -sinf(elevation_rad), 0.0f),
			vec4(0.0f, sinf(elevation_rad), cosf(elevation_rad), 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	transform = mul_mm4(&next, &transform);
	// projection: adjust for vertical and horizontal field of view,
	// put -z into w for perspective divide, w to z to store depth as -1/z
	next = mat4(
			vec4(2 * (float)data->image->height / data->image->width, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, 2, 0.0f, 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f),
			vec4(0.0f, 0.0f, -1.0f, 0.0f));
	transform = mul_mm4(&next, &transform);
	clear_image(data->image);
	draw_mesh(data->image, &data->mesh, &transform);
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
	{
		fdf_data->azimuth_deg = -45;
		fdf_data->elevation_deg = 35.2643897f;
		fdf_data->camera_pos = vec4(10, 10, 10, 1);
	}
	else if (key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
	{
		if (key_data.key == MLX_KEY_LEFT)
			fdf_data->azimuth_deg += 5;
		else if (key_data.key == MLX_KEY_RIGHT)
			fdf_data->azimuth_deg -= 5;
		else if (key_data.key == MLX_KEY_UP)
			fdf_data->elevation_deg += 2;
		else if (key_data.key == MLX_KEY_DOWN)
			fdf_data->elevation_deg -= 2;
		else if (key_data.key == MLX_KEY_W)
			move_camera(fdf_data, 0, 0, -1);
		else if (key_data.key == MLX_KEY_S)
			move_camera(fdf_data, 0, 0, 1);
		else if (key_data.key == MLX_KEY_A)
			move_camera(fdf_data, -1, 0, 0);
		else if (key_data.key == MLX_KEY_D)
			move_camera(fdf_data, 1, 0, 0);
		else if (key_data.key == MLX_KEY_Q)
			move_camera(fdf_data, 0, 1, 0);
		else if (key_data.key == MLX_KEY_Z)
			move_camera(fdf_data, 0, -1, 0);
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
		if (!fdf_data->image)
		{
			mlx_close_window(mlx);
			return ;
		}
		mlx_image_to_window(mlx, fdf_data->image, 0, 0);
		fdf_data->need_redraw = true;
	}
	if (fdf_data->need_redraw)
		draw_with_angle(fdf_data);
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
	data.azimuth_deg = -45;
	data.elevation_deg = 35.2643897f;
	data.camera_pos = vec4(10, 10, 10, 1);
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
