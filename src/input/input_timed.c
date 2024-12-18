/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_timed.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:59:50 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/18 20:08:50 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "input_internal.h"
#include "camera.h"

static void	input_camera_move(t_fdf *fdf, int arg, float time)
{
	camera_move(&fdf->camera, (t_camera_dir)arg, time * 10);
}

static void	input_camera_rotate(t_fdf *fdf, int arg, float time)
{
	camera_rotate(&fdf->camera, (t_camera_dir)arg, time * 90);
}

static const t_input_timed	g_input_timed[] = {
{MLX_KEY_W,		input_camera_move, CAM_FRONT},
{MLX_KEY_S,		input_camera_move, CAM_BACK},
{MLX_KEY_A,		input_camera_move, CAM_LEFT},
{MLX_KEY_D,		input_camera_move, CAM_RIGHT},
{MLX_KEY_Q,		input_camera_move, CAM_UP},
{MLX_KEY_Z,		input_camera_move, CAM_DOWN},
{MLX_KEY_LEFT,	input_camera_rotate, CAM_LEFT},
{MLX_KEY_RIGHT,	input_camera_rotate, CAM_RIGHT},
{MLX_KEY_UP,	input_camera_rotate, CAM_UP},
{MLX_KEY_DOWN,	input_camera_rotate, CAM_DOWN},
};

void	input_timed(t_fdf *fdf)
{
	size_t			i;
	t_input_timed	input;

	i = 0;
	while (i < sizeof g_input_timed / sizeof(*g_input_timed))
	{
		input = g_input_timed[i++];
		if (mlx_is_key_down(fdf->mlx, input.key))
		{
			input.func(fdf, input.arg, fdf->mlx->delta_time);
			fdf->need_redraw = true;
		}
	}
}
