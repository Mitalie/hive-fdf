/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/18 18:17:09 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdlib.h>
#include "MLX42/MLX42.h"
#include "z_image.h"
#include "camera.h"
#include "map.h"
#include "input.h"

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
		fdf_reset(fdf);
	else if (key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
	{
		if (key_data.key == MLX_KEY_P)
			camera_next_mode(&fdf->camera);
		else if (key_data.key == MLX_KEY_KP_ADD)
			fdf->camera.zoom_exp += 0.5f;
		else if (key_data.key == MLX_KEY_KP_SUBTRACT)
			fdf->camera.zoom_exp -= 0.5f;
		else if (key_data.key == MLX_KEY_PAGE_UP)
			fdf->height_scale_exp += 0.5f;
		else if (key_data.key == MLX_KEY_PAGE_DOWN)
			fdf->height_scale_exp -= 0.5f;
		else if (key_data.key == MLX_KEY_F)
			fdf_fit(fdf);
	}
	fdf->need_redraw = true;
}

static void	loop_hook(void *param)
{
	t_fdf	*fdf;

	fdf = param;
	fdf_recreate_image(fdf);
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
	fdf.image = 0;
	fdf_recreate_image(&fdf);
	fdf_reset(&fdf);
	mlx_key_hook(fdf.mlx, key_hook, &fdf);
	mlx_loop_hook(fdf.mlx, loop_hook, &fdf);
	mlx_loop(fdf.mlx);
	if (fdf.image)
		z_image_delete(fdf.mlx, fdf.image);
	mlx_terminate(fdf.mlx);
	free(fdf.mesh.lines);
	free(fdf.mesh.vertices);
}
