/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/19 15:20:09 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "line.h"
#include "math.h"
#include "map.h"
#include "dummy_map.h"

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*image;
}	t_data;

static const t_mat4f g_transform = {{
	(t_vec4f){{0.7071f, -0.7071f, 0.0f, 0.0f}},
	(t_vec4f){{0.7071f, 0.7071f, 0.0f, 0.0f}},
	(t_vec4f){{0.0f, -0.5f, 1.0f, 0.0f}},
	(t_vec4f){{0.0f, 0.0f, 0.0f, 1.0f}},
}};

int	main(void)
{
	t_data	data;

	data.mlx = mlx_init(1500, 1125, "fdf", true);
	if (!data.mlx)
		return (1);
	data.image = mlx_new_image(data.mlx, data.mlx->width, data.mlx->height);
	if (data.image)
	{
		draw_map(data.image, get_dummy_map(), &g_transform);
		mlx_image_to_window(data.mlx, data.image, 0, 0);
	}
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
}
