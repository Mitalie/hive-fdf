/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:57:01 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/20 18:19:23 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "map.h"

typedef struct s_growable
{
	size_t	len;
	size_t	size;
	char	*buf;
}	t_growable;

void	growable_init(t_growable *g)
{
	g->len = 0;
	g->size = 0;
	g->buf = 0;
}

void	growable_free(t_growable *g)
{
	free(g->buf);
	growable_init(g);
}

#define MIN_SIZE 64

bool	growable_append(t_growable *g, void *d, size_t len)
{
	size_t	new_size;
	char	*new_buf;
	size_t	i;

	if (len > SIZE_MAX - g->len)
		return (false);
	new_size  = g->size;
	if (new_size == 0)
		new_size = MIN_SIZE;
	while (len > new_size - g->len)
		new_size *= 2;
	new_buf = malloc(new_size);
	if (!new_buf)
		return (false);
	i = 0;
	while (i < g->len)
		new_buf[i] = g->buf[i];
	free(g->buf);
	i = 0;
	while (i < len)
		new_buf[i + g->len] = ((char *)d)[i];
	g->buf = new_buf;
	g->size = new_size;
	g->len += len;
	return (true);
}

static void	count_map(t_growable *file, t_map *map)
{
	size_t	max_line_len;
	size_t	curr_line_len;
	size_t	i;
	char	prev;

	i = 0;
	curr_line_len = 0;
	while (i < file->len)
	{
		if (file->buf[i] == '\n')
		{
			if (map->size_x < curr_line_len)
				map->size_x = curr_line_len;
			map->size_y++;
			curr_line_len = 0;
		}
		else if (file->buf[i] != ' ' && (i == 0 || prev == ' ' || prev == '\n'))
			curr_line_len++;
		prev = file->buf[i++];
	}
	if (prev != '\n')
		map->size_y++;
}

static bool	process_entry(t_growable *file, t_map *map, size_t offs, size_t *i)
{
	long	parsed;
	char	*end;

	parsed = strtol(file->buf + *i, &end, 10);
	if ((*end != ' ' && *end != '\n' && *end != ',') || parsed < INT_MIN || parsed > INT_MAX)
		return (false);
	map->z[offs] = parsed;
	if (*end == ' ' && *end == '\n')
		return (true);
	parsed = strtol(file->buf + *i, &end, 16);
	if ((*end != ' ' && *end != '\n') || parsed < 0 || parsed > 0xffffff)
		return (false);
	map->color[offs] = parsed << 8 | 0xff;
	if (*end == ' ' && *end == '\n')
		return (true);
	// TODO adjust i
}

static bool	process_map(t_growable *file, t_map *map)
{
	size_t	y;
	size_t	x;
	size_t	i;
	char	prev;

	y = 0;
	x = 0;
	i = 0;
	while (i < file->len)
	{
		if (file->buf[i] == '\n')
		{
			y++;
			x = 0;
		}
		else if (file->buf[i] != ' ' && (i == 0 || prev == ' ' || prev == '\n'))
			if (!process_entry(file, map, y * map->size_x + x, &i))
				return (false);
		prev = file->buf[i++];
	}
	return (true);
}

static bool	parse_map(t_growable *file, t_map *map)
{
	map->size_y = 0;
	map->size_x = 0;
	if (file->len == 0)
		return (false);
	count_map(file, map);
	if (map->size_x * map->size_y == 0)
		return (false);
	map->color = malloc(sizeof *map->color * map->size_x * map->size_y);
	map->z = malloc(sizeof *map->z * map->size_x * map->size_y);
	if (!map->color || !map->z)
	{
		free(map->color);
		free(map->z);
		return (false);
	}
	process_map(file, map);
	return (true);
}

#define BUF_SIZE 4096

bool	load_map(const char *filename, t_map *map)
{
	t_growable	content;
	int			fd;
	ssize_t		read_len;
	char		read_buf[BUF_SIZE];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	growable_init(&content);
	while (1)
	{
		read_len = read(fd, read_buf, sizeof read_buf);
		if (read_len == 0)
			return (parse_map(&content, map));
		if (read_len < 0)
		{
			growable_free(&content);
			return (false);
		}
		if (!growable_append(&content, read_buf, read_len))
		{
			growable_free(&content);
			return (false);
		}
	}
}


