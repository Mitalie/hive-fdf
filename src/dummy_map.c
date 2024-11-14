/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:47:12 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/14 18:10:21 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dummy_map.h"

#define X 10
#define Y 5

static const uint32_t	g_dummy_z[X * Y] = {
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

static const t_map		g_dummy_map = {
	X,
	Y,
	(uint32_t *)g_dummy_z,
	(uint32_t *)g_dummy_color,
};

t_map	*get_dummy_map(void)
{
	return ((t_map *)&g_dummy_map);
}
