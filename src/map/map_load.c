/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:10:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:48:01 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "map_internal.h"

#include <stdbool.h>
#include <stddef.h>

#include "file_reader.h"
#include "mesh.h"
#include "parse_int.h"

static bool	map_load_parse_entry(t_file_reader *reader, t_map_entry *entry)
{
	size_t	len;
	size_t	parsed_len;

	entry->color = 0xffffffff;
	len = reader->end - reader->start;
	if (!parse_int32(reader->start, len, &parsed_len, &entry->height))
		return (false);
	reader->start += parsed_len;
	if (reader->start == reader->end
		|| *reader->start == ' ' || *reader->start == '\n')
		return (true);
	if (*reader->start == ',')
	{
		reader->start++;
		len = reader->end - reader->start;
		if (!parse_uint32_hex(reader->start, len, &parsed_len, &entry->color))
			return (false);
		reader->start += parsed_len;
		entry->color = entry->color << 8 | 0xff;
		if (reader->start == reader->end
			|| *reader->start == ' ' || *reader->start == '\n')
			return (true);
	}
	return (false);
}

// "-2147483648,0xffffff" is 20 characters, add one for space/newline
#define MAX_ENTRY_LEN 21

static bool	map_load_next_entry(t_file_reader *reader, t_map_entry *entry)
{
	entry->is_end = false;
	entry->is_newline = false;
	if (!file_reader_fill_buffer(reader, MAX_ENTRY_LEN))
		return (false);
	while (reader->start < reader->end && reader->start[0] == ' ')
	{
		reader->start++;
		if (!file_reader_fill_buffer(reader, MAX_ENTRY_LEN))
			return (false);
	}
	if (reader->start == reader->end)
		entry->is_end = true;
	else if (*reader->start == '\n')
	{
		reader->start++;
		entry->is_newline = true;
	}
	else
		return (map_load_parse_entry(reader, entry));
	return (true);
}

#define READ_BUF_SIZE 1024

bool	map_load(t_mesh *mesh, const char *filename)
{
	t_file_reader	reader;
	t_map_builder	builder;
	t_map_entry		entry;

	if (!file_reader_open(&reader, READ_BUF_SIZE, filename))
		return (false);
	map_builder_init(&builder);
	while (1)
	{
		if (!map_load_next_entry(&reader, &entry))
			break ;
		if (!map_builder_add_entry(&builder, &entry))
			break ;
		if (entry.is_end)
			break ;
	}
	if (file_reader_close(&reader) && entry.is_end)
		return (map_builder_finalize(&builder, mesh));
	map_builder_release(&builder);
	return (false);
}
