/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_image.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:19:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:51:04 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Z_IMAGE_H
# define Z_IMAGE_H

# include <stdint.h>
# include "MLX42/MLX42.h"

# include "vec_mat.h"

typedef struct s_z_image
{
	mlx_image_t	*mlx_img;
	float		*z_buffer;
}	t_z_image;

t_z_image	*z_image_new(mlx_t *mlx, uint32_t width, uint32_t height);
void		z_image_delete(mlx_t *mlx, t_z_image *image);
void		z_image_clear(t_z_image *image, float depth, uint32_t color);
void		z_image_write(t_z_image *image, t_vec4 pos, uint32_t color);

#endif
