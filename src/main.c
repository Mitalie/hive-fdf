/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 20:54:06 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#include <stdbool.h>
#include <stdlib.h>
#include "MLX42/MLX42.h"

#include "input.h"
#include "map.h"
#include "z_image.h"

static void	key_hook(mlx_key_data_t key_data, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	input_key(fdf, key_data);
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
		return (1);
	if (!map_load(&fdf.mesh, argv[1]))
		return (2);
	fdf.mlx = mlx_init(1500, 1125, "fdf", true);
	if (!fdf.mlx)
		return (3);
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
	if (!fdf.image)
		return (4);
}
