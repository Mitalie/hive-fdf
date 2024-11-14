/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/14 15:22:21 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "line.h"
#include "math.h"

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_point		p[3];
}	t_data;

static void	drawlines(t_data *data)
{
	int32_t	x;
	int32_t	y;

	y = 0;
	while (y < data->mlx->height)
	{
		x = 0;
		while (x < data->mlx->width)
			mlx_put_pixel(data->image, x++, y, 0x000000ff);
		y++;
	}
	draw_line(data->image, data->p[0], data->p[1]);
	draw_line(data->image, data->p[1], data->p[2]);
	draw_line(data->image, data->p[2], data->p[0]);
}

static void	mousefunc(mouse_key_t button, action_t action, modifier_key_t mods,
	void *param)
{
	t_data	*data;
	int32_t	mx;
	int32_t	my;

	data = param;
	(void)action, (void)mods;
	mlx_get_mouse_pos(data->mlx, &mx, &my);
	if (mx < 0 || mx > data->mlx->width || my < 0 || my > data->mlx->height)
		return ;
	if (button == MLX_MOUSE_BUTTON_LEFT)
	{
		data->p[0].x = (float)mx / data->mlx->width;
		data->p[0].y = (float)my / data->mlx->height;
	}
	if (button == MLX_MOUSE_BUTTON_RIGHT)
	{
		data->p[1].x = (float)mx / data->mlx->width;
		data->p[1].y = (float)my / data->mlx->height;
	}
	if (button == MLX_MOUSE_BUTTON_MIDDLE)
	{
		data->p[2].x = (float)mx / data->mlx->width;
		data->p[2].y = (float)my / data->mlx->height;
	}
	drawlines(data);
}

int	main(void)
{
	t_data	data;

	data.mlx = mlx_init(1500, 1125, "fdf", true);
	if (!data.mlx)
		return (1);
	data.p[0] = (t_point){0, 0, 0xff0000ff};
	data.p[1] = (t_point){0, 0, 0x00ff00ff};
	data.p[2] = (t_point){0, 0, 0x0000ffff};
	data.image = mlx_new_image(data.mlx, data.mlx->width, data.mlx->height);
	if (data.image)
	{
		mlx_mouse_hook(data.mlx, mousefunc, &data);
		drawlines(&data);
		mlx_image_to_window(data.mlx, data.image, 0, 0);
	}
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
}
