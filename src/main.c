/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:34 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/06 16:54:20 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"

int	main(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(800, 600, "fdf", true);
	if (!mlx)
		return (1);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
