/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:02:44 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/04 18:21:52 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H

# include "MLX42/MLX42.h"
# include "vertex.h"

typedef struct s_line_data
{
	t_vec4		pos_a;
	t_vec4		pos_b;
	float		t_a;
	float		t_b;
	uint32_t	color_a;
	uint32_t	color_b;
}	t_line_data;

void	draw_line(mlx_image_t *image, t_vertex a, t_vertex b);

#endif
