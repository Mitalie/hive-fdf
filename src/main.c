/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/19 15:54:36 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "MLX42/MLX42.h"
#include "line.h"
#include "map.h"
#include "dummy_map.h"

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*image;
	float		angle_deg;
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

static void	draw_with_angle(mlx_image_t *image, t_map *map, float angle_rad)
{
	t_mat4f	transform;

	transform = ((t_mat4f){
			(t_vec4f){{cosf(angle_rad), sinf(angle_rad), 0.0f, 0.0f}},
			(t_vec4f){{-sinf(angle_rad), cosf(angle_rad), 0.0f, 0.0f}},
			(t_vec4f){{0.0f, -0.5f, 1.0f, 0.0f}},
			(t_vec4f){{0.0f, 0.0f, 0.0f, 1.0f}},
		});
	clear_image(image);
	draw_map(image, map, &transform);
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
		fdf_data->angle_deg = -45;
	else if (key_data.key == MLX_KEY_LEFT && (
			key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT))
		fdf_data->angle_deg += 5;
	else if (key_data.key == MLX_KEY_RIGHT && (
			key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT))
		fdf_data->angle_deg -= 5;
	draw_with_angle(fdf_data->image, get_dummy_map(),
		fdf_data->angle_deg / 180 * 3.1415926535);
}

int	main(void)
{
	t_data	data;

	data.mlx = mlx_init(1500, 1125, "fdf", true);
	if (!data.mlx)
		return (1);
	data.image = mlx_new_image(data.mlx, data.mlx->width, data.mlx->height);
	data.angle_deg = -45;
	if (data.image)
	{
		mlx_key_hook(data.mlx, key_hook, &data);
		draw_with_angle(data.image, get_dummy_map(),
			data.angle_deg / 180 * 3.1415926535);
		mlx_image_to_window(data.mlx, data.image, 0, 0);
	}
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
}
