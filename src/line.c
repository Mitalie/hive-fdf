/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:05:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 20:50:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include <math.h>
#include <stdint.h>
#include "color.h"

/*
	A plane can be described by a point p0 on the plane, and a normal vector n
	with n != 0. Distance d of point p from such a plane is
	d = (p - p0) dot n / |n|. Points where d = 0 lie on the plane, d > 0 are on
	the side the normal vector points to, and d < 0 are on the opposite side.

	A line through points a and b with a != b is all points p = a + t(b - a)
	where t is a scalar. If a and b are endpoints of the line, then 0 <= t <= 1.

	Distance of such points from a plane d(t) = (a + t(b - a) - p0) dot n / |n|.

	An intersection of the line and the plane is described by d(t) = 0. Solving
	for t, we get t = (p0 - a) dot n / (b - a) dot n.

	Our clip planes all pass through origin, so the p0 term disappears. We also
	only check side of the plane, not actual distance, so the division by |n|
	can be omitted as it is always nonnegative and doesn't change the sign. The
	equations can be simplified:
	d = p dot n
	t = -(a dot n)/(b dot n - a dot n)

	For each clip plane we check on which side of the plane the line endpoints
	are, and adjust the t value bounds to keep the line on the correct side of
	all planes. If both endpoints are on the wrong side or if the t range
	becomes invalid (start > end), we know the entire line is outside the clip
	volume and skip further processing.
*/

static const t_vec4	g_clip_plane_normals[] = {
	(t_vec4){.v = {0, 0, 0, 1}},	// Behind (perspective):     w >= 0
	(t_vec4){.v = {-1, 0, 0, 1}},	// Right:   x <=  w      w - x >= 0
	(t_vec4){.v = {1, 0, 0, 1}},	// Left:    x >= -w      w + x >= 0
	(t_vec4){.v = {0, 1, 0, 1}},	// Top:     y <=  w      w - y >= 0
	(t_vec4){.v = {0, -1, 0, 1}},	// Bottom:  y >= -w      w + y >= 0
};

static bool	clip(t_vec4 a, t_vec4 b, float *min_t, float *max_t)
{
	size_t	i;
	float	dot_an;
	float	dot_bn;

	i = 0;
	while (i < sizeof g_clip_plane_normals / sizeof(*g_clip_plane_normals)
		&& *min_t <= *max_t)
	{
		dot_an = dot4(a, g_clip_plane_normals[i]);
		dot_bn = dot4(b, g_clip_plane_normals[i]);
		if (dot_an < 0 && dot_bn < 0)
			return (false);
		if (dot_an < 0)
			*min_t = fmaxf(*min_t, -dot_an / (dot_bn - dot_an));
		if (dot_bn < 0)
			*max_t = fminf(*max_t, -dot_an / (dot_bn - dot_an));
		i++;
	}
	return (*min_t <= *max_t);
}

static bool	prepare(t_vertex *a, t_vertex *b)
{
	float	t_a;
	float	t_b;
	t_vec4	new_a;
	t_vec4	new_b;

	t_a = 0;
	t_b = 1;
	if (!clip(a->pos, b->pos, &t_a, &t_b))
		return (false);
	new_a = add4(mul_sv4(1 - t_a, a->pos), mul_sv4(t_a, b->pos));
	new_b = add4(mul_sv4(1 - t_b, a->pos), mul_sv4(t_b, b->pos));
	a->pos = vec4(
			new_a.x / new_a.w,
			new_a.y / new_a.w,
			new_a.z / new_a.w,
			1 / new_a.w);
	b->pos = vec4(
			new_b.x / new_b.w,
			new_b.y / new_b.w,
			new_b.z / new_b.w,
			1 / new_b.w);
	return (true);
}

/*
	TODO: Perspective correct colour interpolation.
*/

static void	line_loop(t_z_image *image, t_vertex a, t_vertex b)
{
	t_vec4		delta;
	float		len;
	int32_t		i;
	t_vec4		pos;

	delta = sub4(b.pos, a.pos);
	len = fmaxf(fabsf(delta.x), fabsf(delta.y));
	i = 0;
	while (i < len)
	{
		pos = add4(a.pos, mul_sv4(i / len, delta));
		z_image_write(image, pos, color_interp(a.color, b.color, i / len));
		i++;
	}
}

/*
	We generally use axes +X = right, +Y = up, +Z = behind but the pixel buffer
	uses +X = right, +Y = down so invert Y here.
*/

void	draw_line(t_z_image *image, t_vertex a, t_vertex b)
{
	if (!prepare(&a, &b))
		return ;
	a.pos.x = floorf((a.pos.x + 1.0f) * 0.5f * image->mlx_img->width) + 0.5f;
	a.pos.y = floorf((-a.pos.y + 1.0f) * 0.5f * image->mlx_img->height) + 0.5f;
	b.pos.x = floorf((b.pos.x + 1.0f) * 0.5f * image->mlx_img->width) + 0.5f;
	b.pos.y = floorf((-b.pos.y + 1.0f) * 0.5f * image->mlx_img->height) + 0.5f;
	line_loop(image, a, b);
}
