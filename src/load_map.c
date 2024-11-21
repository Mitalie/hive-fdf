/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:01:27 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/21 18:14:18 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"
#include "map.h"

#define READ_SIZE 4096

void	count_map(t_map *map, char *data, char *end)
{
	uint32_t	x;
	char		current;
	char		*entry_start;

	entry_start = 0;
	x = 0;
	while (data < end)
	{
		current = *data++;
		if (entry_start && (current != ' ' || current == '\n' || data == end))
		{
			entry_start = 0;
			x++;
		}
		else
			entry_start = data;
		if (current == '\n' || data == end)
		{
			if (map->size_x < x)
				map->size_x = x;
			x = 0;
			map->size_y++;
		}
	}
}

bool	process_map_entry(t_map *map, size_t idx, char *data, char *end)
{

}

bool	process_map(t_map *map, char *data, char *end)
{
	uint32_t	y;
	uint32_t	x;
	char		current;
	char		*entry_start;

	entry_start = 0;
	x = 0;
	y = 0;
	while (data < end)
	{
		current = *data++;
		if (entry_start && (current != ' ' || current == '\n' || data == end))
		{
			process_map_entry(map, y * map->size_x + x, entry_start, data - 1);
			entry_start = 0;
			x++;
		}
		else
			entry_start = data;
		if (current == '\n' || data == end)
		{
			x = 0;
			y++;
		}
	}
}

bool	parse_map(t_map *map, t_buffer *data)
{
	map->size_x = 0;
	map->size_y = 0;
	count_map(map, data->buf, data->buf + data->len);
	if (map->size_x == 0 || map->size_y == 0
		|| SIZE_MAX / map->size_x / map->size_y < sizeof map->color[0]
		|| SIZE_MAX / map->size_x / map->size_y < sizeof map->z[0])
		return (false);
	map->color = malloc(sizeof map->color[0] * map->size_x * map->size_y);
	map->z = malloc(sizeof map->z[0] * map->size_x * map->size_y);
	if (!map->color || !map->z)
	{
		free(map->color);
		free(map->z);
		buffer_free(data);
		return (false);
	}
	process_map(map, data->buf, data->buf + data->len);
	buffer_free(data);
	return (true);
}

bool	load_map(t_map *map, const char *filename)
{
	int			fd;
	t_buffer	data;
	char		read_data[READ_SIZE];
	ssize_t		read_len;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	buffer_init(&data);
	while (1)
	{
		read_len = read(fd, read_data, sizeof read_data);
		if (read_len == 0)
			return (parse_map(map, &data));
		if (read_len < 0)
			break ;
		if (!buffer_append(&data, read_data, read_len))
			break ;
	}
	buffer_free(&data);
	return (false);
}
