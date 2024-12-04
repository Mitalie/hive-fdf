/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:05:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/04 18:22:54 by amakinen         ###   ########.fr       */
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

	We calculate each plane clipping with original endpoints rather than using t
	bounds from previous planes. This seems like it could miss early rejection
	opportunities when both adjusted endpoints are on the wrong side of the
	current plane, but those cases can be detected as invalid t ranges.
*/

// All planes through origin, so we only need plane normals
static const t_vec4	g_clip_plane_normals[] = {
	(t_vec4){{0, 0, 0, 1}},	// Behind (perspective):     w >= 0
	(t_vec4){{-1, 0, 0, 1}},	// Right:   x <=  w      w - x >= 0
	(t_vec4){{1, 0, 0, 1}},	// Left:    x >= -w      w + x >= 0
	(t_vec4){{0, 1, 0, 1}},	// Top:     y <=  w      w - y >= 0
	(t_vec4){{0, -1, 0, 1}},	// Bottom:  y >= -w      w + y >= 0
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

static bool	prepare(t_line_data *l, t_vertex a, t_vertex b)
{
	t_vec4	new_a;
	t_vec4	new_b;

	l->t_a = 0;
	l->t_b = 1;
	if (!clip(a.pos, b.pos, &l->t_a, &l->t_b))
		return (false);
	new_a = add4(mul_sv4(1 - l->t_a, a.pos), mul_sv4(l->t_a, b.pos));
	new_b = add4(mul_sv4(1 - l->t_b, a.pos), mul_sv4(l->t_b, b.pos));
	l->pos_a = vec4(
			new_a.x / new_a.w,
			new_a.y / new_a.w,
			new_a.z / new_a.w,
			1 / new_a.w);
	l->pos_b = vec4(
			new_b.x / new_b.w,
			new_b.y / new_b.w,
			new_b.z / new_b.w,
			1 / new_b.w);
	l->color_a = a.color;
	l->color_b = b.color;
	return (true);
}

/*
	TODO: Perspective correct colour interpolation.
	TODO: Doing Z clipping inside the line drawing loop is suboptimal. Consider
		clipping before calling line_horizontal/line_vertical.
	TODO: Implement Z buffer and Z test.
*/

static void	line_horizontal(mlx_image_t *image, t_line_data *l)
{
	float	slope;
	float	xs;
	float	xe;
	float	x;
	float	y;

	slope = (l->pos_b.y - l->pos_a.y) / (l->pos_b.x - l->pos_a.x);
	xs = l->pos_a.x;
	xe = l->pos_b.x;
	x = xs;
	while ((xs <= xe && x <= xe) || (xs > xe && x >= xe))
	{
		y = l->pos_a.y + slope * (x - xs);
		if (x >= 0 && x < image->width && y >= 0 && y < image->height)
			mlx_put_pixel(image, x, y,
				color_interp(l->color_a, l->color_b, (x - xs) / (xe - xs)));
		else
			;
		if (xe < xs)
			x--;
		else
			x++;
	}
}

static void	line_vertical(mlx_image_t *image, t_line_data *l)
{
	float	slope;
	float	ys;
	float	ye;
	float	y;
	float	x;

	slope = (l->pos_b.x - l->pos_a.x) / (l->pos_b.y - l->pos_a.y);
	ys = l->pos_a.y;
	ye = l->pos_b.y;
	y = ys;
	while ((ys <= ye && y <= ye) || (ys > ye && y >= ye))
	{
		x = l->pos_a.x + slope * (y - ys);
		if (x >= 0 && x < image->width && y >= 0 && y < image->height)
			mlx_put_pixel(image, x, y,
				color_interp(l->color_a, l->color_b, (y - ys) / (ye - ys)));
		else
			;
		if (ye < ys)
			y--;
		else
			y++;
	}
}

/*
	We generally use axes +X = right, +Y = up, +Z = behind but the pixel buffer
	uses +X = right, +Y = down so invert Y here.
*/

void	draw_line(mlx_image_t *image, t_vertex a, t_vertex b)
{
	t_line_data	l;

	if (!prepare(&l, a, b))
		return ;
	l.pos_a.x = floorf((l.pos_a.x + 1.0f) * 0.5f * image->width) + 0.5f;
	l.pos_a.y = floorf((-l.pos_a.y + 1.0f) * 0.5f * image->height) + 0.5f;
	l.pos_b.x = floorf((l.pos_b.x + 1.0f) * 0.5f * image->width) + 0.5f;
	l.pos_b.y = floorf((-l.pos_b.y + 1.0f) * 0.5f * image->height) + 0.5f;
	if (fabsf(l.pos_a.y - l.pos_b.y) <= fabsf(l.pos_a.x - l.pos_b.x))
		line_horizontal(image, &l);
	else
		line_vertical(image, &l);
}
