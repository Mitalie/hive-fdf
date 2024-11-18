/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:47:30 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/18 20:18:58 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "line.h"

#define INSET 0.1f

static t_point	make_point(t_map *map, uint32_t x, uint32_t y)
{
	return ((t_point){
		{{
			INSET + (1.0f - INSET - INSET) * x / (map->size_x - 1),
			INSET + (1.0f - INSET - INSET) * y / (map->size_y - 1),
		}},
		map->color[y * map->size_x + x],
	});
}

static void	draw_column(mlx_image_t *image, t_map *map, uint32_t x)
{
	uint32_t	y;
	t_point		curr;
	t_point		next;

	y = 0;
	next = make_point(map, x, y);
	while (++y < map->size_y)
	{
		curr = next;
		next = make_point(map, x, y);
		draw_line(image, curr, next);
	}
}

static void	draw_row(mlx_image_t *image, t_map *map, uint32_t y)
{
	uint32_t	x;
	t_point		curr;
	t_point		next;

	x = 0;
	next = make_point(map, x, y);
	while (++x < map->size_x)
	{
		curr = next;
		next = make_point(map, x, y);
		draw_line(image, curr, next);
	}
}

void	draw_map(mlx_image_t *image, t_map *map)
{
	uint32_t	i;

	i = 0;
	while (i < map->size_y)
		draw_row(image, map, i++);
	i = 0;
	while (i < map->size_x)
		draw_column(image, map, i++);
}
