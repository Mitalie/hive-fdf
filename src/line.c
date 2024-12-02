/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:05:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/02 20:59:09 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include <math.h>
#include <stdint.h>

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
		out |= (uint32_t)fminf(ta + tb, 0xff) << i;
		i += 8;
	}
	return (out);
}

/*
	TODO: Perspective correct colour interpolation.
	TODO: Doing Z clipping inside the line drawing loop is suboptimal. Consider
		clipping before calling line_horizontal/line_vertical.
	TODO: Implement Z buffer and Z test.
*/

#include <stdio.h>

static void	line_horizontal(mlx_image_t *image, t_vertex a, t_vertex b)
{
	float		slope;
	int32_t		i;
	int32_t		iend;
	float		iymin;
	float		iymax;

	slope = (b.pos.y - a.pos.y) / (b.pos.x - a.pos.x);
	if ((slope >= 0 && a.pos.y >= image->height) || (slope <= 0 && a.pos.y < 0))
		return ;
	iymin = roundf(-a.pos.y / slope);
	iymax = roundf((image->height - fabsf(slope) - a.pos.y) / slope);
	i = fmaxf(fmaxf(0.0f, -a.pos.x), fminf(iymin, iymax));
	iend = fminf(fminf(image->width, b.pos.x) - a.pos.x, fmaxf(iymin, iymax));
	while (i < iend)
	{
		if (a.pos.z + i * (b.pos.z - a.pos.z) / (b.pos.x - a.pos.x) > 0)
			mlx_put_pixel(image, a.pos.x + i, a.pos.y + i * slope,
				color_interp(a.color, b.color, i / (b.pos.x - a.pos.x)));
		i++;
	}
}

static void	line_vertical(mlx_image_t *image, t_vertex a, t_vertex b)
{
	float		slope;
	int32_t		i;
	int32_t		iend;
	float		ixmin;
	float		ixmax;

	slope = (b.pos.x - a.pos.x) / (b.pos.y - a.pos.y);
	if ((slope >= 0 && a.pos.x >= image->width) || (slope <= 0 && a.pos.x < 0))
		return ;
	ixmin = roundf(-a.pos.x / slope);
	ixmax = roundf((image->width - fabsf(slope) - a.pos.x) / slope);
	i = fmaxf(fmaxf(0.0f, -a.pos.y), fminf(ixmin, ixmax));
	iend = fminf(fminf(image->height, b.pos.y) - a.pos.y, fmaxf(ixmin, ixmax));
	while (i < iend)
	{
		if (a.pos.z + i * (b.pos.z - a.pos.z) / (b.pos.y - a.pos.y) > 0)
			mlx_put_pixel(image, a.pos.x + i * slope, a.pos.y + i,
				color_interp(a.color, b.color, i / (b.pos.y - a.pos.y)));
		i++;
	}
}

/*
	TODO: Do clipping before divide. Without abs, negative w negates X and Y
		which makes them totally wrong. If entire line is behind, nothing is
		visible because of z-clipping, but if part of the line is in front it
		is drawn from one correct point towards one wrong point.
*/

static t_vec4f	perspective_divide(t_vec4f pos)
{
	float	w_recip;

	w_recip = 1.0f / pos.w;
	return (vec4f(
			pos.x * fabsf(w_recip),
			pos.y * fabsf(w_recip),
			pos.z * w_recip,
			w_recip));
}

/*
	We generally use axes +X = right, +Y = up, +Z = behind but the pixel buffer
	uses +X = right, +Y = down so invert Y here.
*/

void	draw_line(mlx_image_t *image, t_vertex a, t_vertex b)
{
	a.pos = perspective_divide(a.pos);
	b.pos = perspective_divide(b.pos);
	a.pos.x = floorf((a.pos.x + 1.0f) * 0.5f * image->width) + 0.5f;
	a.pos.y = floorf((-a.pos.y + 1.0f) * 0.5f * image->height) + 0.5f;
	b.pos.x = floorf((b.pos.x + 1.0f) * 0.5f * image->width) + 0.5f;
	b.pos.y = floorf((-b.pos.y + 1.0f) * 0.5f * image->height) + 0.5f;
	if (fabsf(a.pos.y - b.pos.y) <= fabsf(a.pos.x - b.pos.x))
	{
		if (a.pos.x <= b.pos.x)
			line_horizontal(image, a, b);
		else
			line_horizontal(image, b, a);
	}
	else
	{
		if (a.pos.y <= b.pos.y)
			line_vertical(image, a, b);
		else
			line_vertical(image, b, a);
	}
}
