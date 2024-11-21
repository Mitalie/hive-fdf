/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_mesh.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:47:12 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/21 19:02:47 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dummy_mesh.h"
#include <stdlib.h>

#define X 10
#define Y 5

static const int32_t	g_dummy_z[X * Y] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 4, 4, 3, 3, 2, 2, 1, 1,
	0, 2, 2, 1, 0, 0, 0, 0, 0, 0,
	0, -1, -2, 0, 0, 0, -9, 9, 0, 0,
	0, -4, -3, 0, 0, 0, 0, 0, 0, 0,
};

static const uint32_t	g_dummy_color[X * Y] = {
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xff0000ff, 0xff3f00ff, 0xff7f00ff, 0xffbf00ff, 0xffff00ff,
	0xffff00ff, 0xbfff00ff, 0x7fff00ff, 0x3fff00ff, 0x00ff00ff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0x0000ffff, 0x00ffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
};

static t_vertex	make_vertex(uint32_t x, uint32_t y)
{
	t_vertex	vertex;

	vertex.pos.x = (2.0f * x - (X - 1)) / 8.0f;
	vertex.pos.y = (2.0f * y - (Y - 1)) / 8.0f;
	vertex.pos.z = g_dummy_z[y * X + x] / 8.0f;
	vertex.pos.w = 1;
	vertex.color = g_dummy_color[y * X + x];
	return (vertex);
}

static t_line	make_line_horizontal(uint32_t x, uint32_t y)
{
	t_line	line;

	line.a = y * X + x;
	line.b = y * X + x - 1;
	return (line);
}

static t_line	make_line_vertical(uint32_t x, uint32_t y)
{
	t_line	line;

	line.a = y * X + x;
	line.b = (y - 1) * X + x;
	return (line);
}

bool	load_dummy_mesh(t_mesh *mesh)
{
	uint32_t	y;
	uint32_t	x;
	uint32_t	line_idx;

	mesh->vertices = malloc(sizeof mesh->vertices[0] * X * Y);
	mesh->n_lines = 2 * X * Y - X - Y;
	mesh->lines = malloc(sizeof mesh->lines[0] * mesh->n_lines);
	if (!mesh->vertices || !mesh->lines)
		return (false);
	line_idx = 0;
	y = 0;
	while (y < Y)
	{
		x = 0;
		while (x < X)
		{
			mesh->vertices[y * X + x] = make_vertex(x, y);
			if (x > 0)
				mesh->lines[line_idx++] = make_line_horizontal(x, y);
			if (y > 0)
				mesh->lines[line_idx++] = make_line_vertical(x, y);
			x++;
		}
		y++;
	}
	return (true);
}
