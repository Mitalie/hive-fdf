/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:02:44 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/14 15:04:13 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H

# include <stdint.h>
# include "MLX42/MLX42.h"

typedef struct s_point
{
	float		x;
	float		y;
	uint32_t	color;
}	t_point;

typedef struct s_point_int
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;
}	t_point_int;

void	draw_line(mlx_image_t *image, t_point a, t_point b);

#endif
