/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:05:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/11 17:42:37 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include "line_internal.h"
#include <math.h>
#include <stdint.h>
#include "color.h"

/*
	The part of the world that is visible on the screen is the clip volume,
	defined by several planes. Each plane divides space in two halves, one of
	which is outside the view. There is no need to draw anything that is not
	visible, so determine which part (if any) of each line is visible.

	A plane can be described by a point p0 on the plane, and a normal vector n
	with n != 0. Distance d of point p from such a plane is
	d = (p - p0) dot n / |n|. Points where d = 0 lie on the plane, d > 0 are on
	the side the normal vector points to, and d < 0 are on the opposite side.

	A line through points a and b with a != b is all points p = a + t(b - a)
	where t is a scalar. If a and b are endpoints of the line, then 0 <= t <= 1.

	Distance of such points from a plane d(t) = (a + t(b - a) - p0) dot n / |n|.

	An intersection of the line and the plane is described by d(t) = 0. Solving
	for t, we get t = (p0 - a) dot n / (b - a) dot n.

	For each clip plane we check on which side of the plane the line endpoints
	are, and adjust the t value bounds to select the part of the line on the
	correct side of all planes. If both endpoints are on the wrong side or if
	the t range becomes invalid (start > end), we know the entire line is
	outside the clip volume and skip further processing.

	Our clip planes all pass through origin, so the p0 term disappears. We also
	only check side of the plane, not actual distance, so the division by |n|
	can be omitted as it is always positive and doesn't change the sign. The
	equations can be simplified:
	d = p dot n
	t = -(a dot n)/(b dot n - a dot n)
*/

static const t_vec4	g_clip_plane_normals[] = {
	(t_vec4){.v = {0, 0, 0, 1}},	// Behind (perspective):     w >= 0
	(t_vec4){.v = {-1, 0, 0, 1}},	// Right:   x <=  w      w - x >= 0
	(t_vec4){.v = {1, 0, 0, 1}},	// Left:    x >= -w      w + x >= 0
	(t_vec4){.v = {0, 1, 0, 1}},	// Top:     y <=  w      w - y >= 0
	(t_vec4){.v = {0, -1, 0, 1}},	// Bottom:  y >= -w      w + y >= 0
};

static bool	line_calculate_clip(t_line_data *line)
{
	size_t	i;
	float	dot_an;
	float	dot_bn;

	i = 0;
	line->ta = 0;
	line->tb = 1;
	while (i < sizeof g_clip_plane_normals / sizeof(*g_clip_plane_normals)
		&& line->ta <= line->tb)
	{
		dot_an = dot4(line->a.pos, g_clip_plane_normals[i]);
		dot_bn = dot4(line->b.pos, g_clip_plane_normals[i]);
		if (dot_an < 0 && dot_bn < 0)
			return (false);
		if (dot_an < 0)
			line->ta = fmaxf(line->ta, -dot_an / (dot_bn - dot_an));
		if (dot_bn < 0)
			line->tb = fminf(line->tb, -dot_an / (dot_bn - dot_an));
		i++;
	}
	return (line->ta <= line->tb);
}

/*
	Calculate the new endpoints based on the clipped t values, then project the
	coordinates from view space into screen space by dividing by w. The inverse
	w values are stored and the t values are also divided to allow view space
	interpolation of t in drawing loop.
*/
static void	line_normalize(t_line_data *line)
{
	t_vec4	clipped_a;
	t_vec4	clipped_b;
	float	wa;
	float	wb;

	clipped_a = lerp4(line->a.pos, line->b.pos, line->ta);
	clipped_b = lerp4(line->a.pos, line->b.pos, line->tb);
	wa = clipped_a.w;
	wb = clipped_b.w;
	line->a.pos = vec4(
			clipped_a.x / wa,
			clipped_a.y / wa,
			clipped_a.z / wa,
			1 / wa);
	line->b.pos = vec4(
			clipped_b.x / wb,
			clipped_b.y / wb,
			clipped_b.z / wb,
			1 / wb);
	line->ta /= wa;
	line->tb /= wb;
}

/*
	Translate [-1, 1] ranges to [0, width] and [0, height], then snap values
	to pixel centers. Our positive Y is up, but the pixel buffer needs positive
	Y down so invert Y here.
*/
static void	line_to_pixel_coords(
	t_line_data *line, uint32_t width, uint32_t height)
{
	line->a.pos.x = 0.5f + floorf((line->a.pos.x + 1) * 0.5f * width);
	line->a.pos.y = 0.5f + floorf((-line->a.pos.y + 1) * 0.5f * height);
	line->b.pos.x = 0.5f + floorf((line->b.pos.x + 1) * 0.5f * width);
	line->b.pos.y = 0.5f + floorf((-line->b.pos.y + 1) * 0.5f * height);
}

/*
	Position on screen is interpolated directly with i / len. The t value along
	the line is interpolated in view space by interpolating	t/w (stored in ta/tb)
	and 1/w (stored in a/b.pos.w) and calculating t = t/w*w = (t/w)/(1/w). This
	makes the segments closer to camera appear longer in perspective projection.
*/
static void	line_draw_loop(t_z_image *image, t_line_data *line)
{
	t_vec4		delta;
	float		len;
	uint32_t	i;
	t_vec4		pos;
	float		t;

	delta = sub4(line->b.pos, line->a.pos);
	len = fmaxf(fabsf(delta.x), fabsf(delta.y));
	i = 0;
	while (i < len)
	{
		pos = lerp4(line->a.pos, line->b.pos, i / len);
		t = ((1 - i / len) * line->ta + i / len * line->tb) / pos.w;
		z_image_write(image, pos,
			color_interp(line->a.color, line->b.color, t));
		i++;
	}
}

void	draw_line(t_z_image *image, t_vertex a, t_vertex b)
{
	t_line_data	line;

	line.a = a;
	line.b = b;
	if (!line_calculate_clip(&line))
		return ;
	line_normalize(&line);
	line_to_pixel_coords(&line, image->mlx_img->width, image->mlx_img->height);
	line_draw_loop(image, &line);
}
