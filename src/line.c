/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:05:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/14 15:04:13 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"

static uint32_t	color_interp(uint32_t a, uint32_t b, float t)
{
	uint32_t	i;
	uint32_t	out;
	float		ta;
	float		tb;

	i = 0;
	out = 0;
	while (i < 32)
	{
		ta = (a >> i & 0xff) * (1 - t);
		tb = (b >> i & 0xff) * t;
		out |= (uint8_t)(ta + tb) << i;
		i += 8;
	}
	return (out);
}

static void	line_horizontal(mlx_image_t *image, t_point_int a, t_point_int b)
{
	uint32_t	dx;
	uint32_t	i;
	float		slope;

	dx = b.x - a.x;
	i = 0;
	mlx_put_pixel(image, a.x, a.y, a.color);
	slope = ((float)b.y - a.y) / dx;
	while (i++ < dx)
		mlx_put_pixel(image, a.x + i, a.y + i * slope + 0.5f,
			color_interp(a.color, b.color, (float)i / dx));
}

static void	line_vertical(mlx_image_t *image, t_point_int a, t_point_int b)
{
	uint32_t	dy;
	uint32_t	i;
	float		slope;

	dy = b.y - a.y;
	i = 0;
	mlx_put_pixel(image, a.x, a.y, a.color);
	slope = ((float)b.x - a.x) / dy;
	while (i++ < dy)
		mlx_put_pixel(image, a.x + i * slope + 0.5f, a.y + i,
			color_interp(a.color, b.color, (float)i / dy));
}

static uint32_t	absdiff(uint32_t a, uint32_t b)
{
	if (a < b)
		return (b - a);
	return (a - b);
}

void	draw_line(mlx_image_t *image, t_point a, t_point b)
{
	t_point_int	ai;
	t_point_int	bi;

	ai.x = a.x * image->width;
	ai.y = a.y * image->height;
	ai.color = a.color;
	bi.x = b.x * image->width;
	bi.y = b.y * image->height;
	bi.color = b.color;
	if (absdiff(ai.y, bi.y) <= absdiff(ai.x, bi.x))
	{
		if (ai.x <= bi.x)
			line_horizontal(image, ai, bi);
		else
			line_horizontal(image, bi, ai);
	}
	else
	{
		if (ai.y <= bi.y)
			line_vertical(image, ai, bi);
		else
			line_vertical(image, bi, ai);
	}
}
