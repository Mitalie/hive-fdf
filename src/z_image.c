/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_image.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:28:27 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:46:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "z_image.h"

#include <stdlib.h>
#include <stdint.h>

#include "MLX42/MLX42.h"

t_z_image	*z_image_new(mlx_t *mlx, uint32_t width, uint32_t height)
{
	t_z_image	*image;

	image = malloc(sizeof(*image));
	if (image)
	{
		image->z_buffer = malloc(sizeof(*image->z_buffer) * width * height);
		if (image->z_buffer)
		{
			image->mlx_img = mlx_new_image(mlx, width, height);
			if (image->mlx_img)
				return (image);
			free(image->z_buffer);
		}
		free(image);
	}
	return (0);
}

void	z_image_delete(mlx_t *mlx, t_z_image *image)
{
	mlx_delete_image(mlx, image->mlx_img);
	free(image->z_buffer);
	free(image);
}

void	z_image_clear(t_z_image *image, float depth, uint32_t color)
{
	uint32_t	height;
	uint32_t	width;
	uint32_t	y;
	uint32_t	x;

	height = image->mlx_img->height;
	width = image->mlx_img->width;
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			mlx_put_pixel(image->mlx_img, x, y, color);
			image->z_buffer[y * width + x] = depth;
			x++;
		}
		y++;
	}
}

void	z_image_write(t_z_image *image, t_vec4 pos, uint32_t color)
{
	uint32_t	width;
	uint32_t	height;
	uint32_t	x;
	uint32_t	y;
	float		*depth_pos;

	width = image->mlx_img->width;
	height = image->mlx_img->height;
	if (pos.x < 0 || pos.y < 0)
		return ;
	x = pos.x;
	y = pos.y;
	if (x >= width || y >= height)
		return ;
	depth_pos = &image->z_buffer[y * width + x];
	if (pos.z < *depth_pos)
		return ;
	*depth_pos = pos.z;
	mlx_put_pixel(image->mlx_img, x, y, color);
}
