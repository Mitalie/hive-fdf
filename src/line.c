/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:05:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/18 19:51:22 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include <math.h>

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

static void	line_horizontal(mlx_image_t *image, t_point a, t_point b)
{
	float		slope;
	int32_t		i;
	int32_t		iend;
	float		iymin;
	float		iymax;

	slope = (b.y - a.y) / (b.x - a.x);
	if ((slope >= 0 && a.y >= image->height) || (slope <= 0 && a.y < 0))
		return ;
	iymin = -a.y / slope;
	iymax = (image->height - fabsf(slope) - a.y) / slope;
	i = fmaxf(fmaxf(0.0f, -a.x), fminf(iymin, iymax));
	iend = fminf(fminf(image->width, b.x) - a.x, fmaxf(iymin, iymax));
	while (i < iend)
	{
		mlx_put_pixel(image, a.x + i, a.y + i * slope,
			color_interp(a.color, b.color, i / (b.x - a.x)));
		i++;
	}
}

static void	line_vertical(mlx_image_t *image, t_point a, t_point b)
{
	float		slope;
	int32_t		i;
	int32_t		iend;
	float		ixmin;
	float		ixmax;

	slope = (b.x - a.x) / (b.y - a.y);
	if ((slope >= 0 && a.x >= image->width) || (slope <= 0 && a.x < 0))
		return ;
	ixmin = -a.x / slope;
	ixmax = (image->width - fabsf(slope) - a.x) / slope;
	i = fmaxf(fmaxf(0.0f, -a.y), fminf(ixmin, ixmax));
	iend = fminf(fminf(image->height, b.y) - a.y, fmaxf(ixmin, ixmax));
	while (i < iend)
	{
		mlx_put_pixel(image, a.x + i * slope, a.y + i,
			color_interp(a.color, b.color, i / (b.y - a.y)));
		i++;
	}
}

void	draw_line(mlx_image_t *image, t_point a, t_point b)
{
	a.x = floorf((a.x + 1.0f) * 0.5f * image->width) + 0.5f;
	a.y = floorf((a.y + 1.0f) * 0.5f * image->height) + 0.5f;
	b.x = floorf((b.x + 1.0f) * 0.5f * image->width) + 0.5f;
	b.y = floorf((b.y + 1.0f) * 0.5f * image->height) + 0.5f;
	if (fabsf(a.y - b.y) <= fabsf(a.x - b.x))
	{
		if (a.x <= b.x)
			line_horizontal(image, a, b);
		else
			line_horizontal(image, b, a);
	}
	else
	{
		if (a.y <= b.y)
			line_vertical(image, a, b);
		else
			line_vertical(image, b, a);
	}
}
