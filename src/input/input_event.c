/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_event.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:30:55 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/18 20:09:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "input_internal.h"

static void	input_simple_action(t_fdf *fdf, int arg)
{
	if (arg == INPUT_CLOSE)
		mlx_close_window(fdf->mlx);
	if (arg == INPUT_RESET)
		fdf_reset(fdf);
	if (arg == INPUT_FIT)
		fdf_fit(fdf);
	if (arg == INPUT_MODE)
		camera_next_mode(&fdf->camera);
}

static void	input_camera_zoom(t_fdf *fdf, int arg)
{
	if (arg == INPUT_UP)
		fdf->camera.zoom_exp += 0.5f;
	if (arg == INPUT_DOWN)
		fdf->camera.zoom_exp -= 0.5f;
}

static void	input_height_scale(t_fdf *fdf, int arg)
{
	if (arg == INPUT_UP)
		fdf->height_scale_exp += 0.5f;
	if (arg == INPUT_DOWN)
		fdf->height_scale_exp -= 0.5f;
}

static const t_input_event	g_input_event[] = {
{MLX_KEY_ESCAPE,		MLX_PRESS,	input_simple_action, INPUT_CLOSE},
{MLX_KEY_R,				MLX_PRESS,	input_simple_action, INPUT_RESET},
{MLX_KEY_F,				MLX_PRESS,	input_simple_action, INPUT_FIT},
{MLX_KEY_M,				MLX_PRESS,	input_simple_action, INPUT_MODE},
{MLX_KEY_KP_ADD,		MLX_PRESS,	input_camera_zoom, INPUT_UP},
{MLX_KEY_KP_ADD,		MLX_REPEAT,	input_camera_zoom, INPUT_UP},
{MLX_KEY_KP_SUBTRACT,	MLX_PRESS,	input_camera_zoom, INPUT_DOWN},
{MLX_KEY_KP_SUBTRACT,	MLX_REPEAT,	input_camera_zoom, INPUT_DOWN},
{MLX_KEY_PAGE_UP,		MLX_PRESS,	input_height_scale, INPUT_UP},
{MLX_KEY_PAGE_UP,		MLX_REPEAT,	input_height_scale, INPUT_UP},
{MLX_KEY_PAGE_DOWN,		MLX_PRESS,	input_height_scale, INPUT_DOWN},
{MLX_KEY_PAGE_DOWN,		MLX_REPEAT,	input_height_scale, INPUT_DOWN},
};

void	input_key(t_fdf *fdf, mlx_key_data_t key_data)
{
	size_t			i;
	t_input_event	input;

	i = 0;
	while (i < sizeof g_input_event / sizeof(*g_input_event))
	{
		input = g_input_event[i++];
		if (key_data.action == input.action && key_data.key == input.key)
		{
			input.func(fdf, input.arg);
			fdf->need_redraw = true;
		}
	}
}
