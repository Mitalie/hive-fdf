/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/02 17:13:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "MLX42/MLX42.h"
#include "line.h"
#include "mesh.h"
#include "map.h"
#include "projection.h"

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_mesh		mesh;
	float		azimuth_deg;
	float		elevation_deg;
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

/*
	1. shrink model and swap axes to match world XYZ
	2. rotate horizontal plane
	3. tilt and scale horizontal axis to match aspect ratio

	Model size and aspect ratio should really be handled by separate projection
	matrix (to be implemented)
*/
static void	draw_with_angle(mlx_image_t *image, t_mesh *mesh,
	float azimuth_rad, float elevation_rad)
{
	t_mat4f	transform;
	t_mat4f	next;

	transform = mat4f(
			/*
			vec4f(1.0f / 20, 0.0f, 0.0f, 0.0f),
			vec4f(0.0f, 0.0f, -1.0f / 20, 0.0f),
			vec4f(0.0f, -1.0f / 20, 0.0f, 0.0f),
			vec4f(0.0f, 0.0f, 0.0f, 1.0f));
			*/
			vec4f(1.0f, 0.0f, 0.0f, 0.0f),
			vec4f(0.0f, 0.0f, -1.0f, 0.0f),
			vec4f(0.0f, -1.0f, 0.0f, 0.0f),
			vec4f(0.0f, 0.0f, 0.0f, 1.0f));
	/*
	next = mat4f(
			vec4f(cosf(azimuth_rad), 0.0f, -sinf(azimuth_rad), 0.0f),
			vec4f(0.0f, 1.0f, 0.0f, 0.0f),
			vec4f(sinf(azimuth_rad), 0.0f, cosf(azimuth_rad), 0.0f),
			vec4f(0.0f, 0.0f, 0.0f, 1.0f));
	transform = mul4f_mat_mat(&next, &transform);
	next = mat4f(
			vec4f((float)image->height / image->width, 0.0f, 0.0f, 0.0f),
			vec4f(0.0f, cosf(elevation_rad), -sinf(elevation_rad), 0.0f),
			vec4f(0.0f, sinf(elevation_rad), cosf(elevation_rad), 0.0f),
			vec4f(0.0f, 0.0f, 0.0f, 1.0f));
	*/
	next = projection_ortho((t_ortho_bounds){20.0f * image->height / image->width, -20.0f * image->height / image->width, 20, -20}, azimuth_rad, elevation_rad);
	transform = mul4f_mat_mat(&next, &transform);
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
	{
		fdf_data->azimuth_deg = -45;
		fdf_data->elevation_deg = 35.2643897f;
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
		draw_with_angle(fdf_data->image, &fdf_data->mesh,
			fdf_data->azimuth_deg / 180 * 3.1415926535,
			fdf_data->elevation_deg / 180 * 3.1415926535);
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
