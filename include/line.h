/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:02:44 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/18 20:11:52 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H

# include <stdint.h>
# include "MLX42/MLX42.h"
# include "vec_mat.h"

typedef struct s_point
{
	t_vec4f		pos;
	uint32_t	color;
}	t_point;

void	draw_line(mlx_image_t *image, t_point a, t_point b);

#endif
