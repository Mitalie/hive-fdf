/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:16:09 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/25 20:03:32 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

bool	map_reader_open(t_map_reader *reader, const char *filename)
{
	reader->start = reader->buf;
	reader->end = reader->buf;
	reader->fd = open(filename, O_RDONLY);
	return (reader->fd >= 0);
}

void	map_reader_close(t_map_reader *reader)
{
	// TODO errors?
	close(reader->fd);
}

static bool	map_reader_fill_buffer(t_map_reader *reader, size_t min_length)
{
	size_t	len;
	size_t	i;
	ssize_t	ret;

	len = reader->end - reader->start;
	if (len >= min_length)
		return (true);
	i = 0;
	while (i < len)
		reader->buf[i++] = *reader->start++;
	reader->start = reader->buf;
	reader->end = reader->start + len;
	while (reader->end < reader->start + min_length)
	{
		ret = read(reader->fd, reader->end, sizeof(reader->buf) - i);
		if (ret < 0 && errno == EINTR)
			continue ;
		if (ret < 0)
			return (false);
		if (ret == 0)
			break ;
		reader->end += ret;
	}
	return (true);
}

static bool	map_reader_parse_entry(t_map_reader *reader, t_map_entry *entry)
{
	size_t	len;
	size_t	parsed_len;

	entry->color = 0xffffffff;
	len = reader->end - reader->start;
	if (!parse_int32(reader->start, len, &parsed_len, &entry->z))
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

bool	map_reader_next_entry(t_map_reader *reader, t_map_entry *entry)
{
	entry->is_end = false;
	entry->is_newline = false;
	if (!map_reader_fill_buffer(reader, MAX_ENTRY_LEN))
		return (false);
	while (reader->start < reader->end && reader->start[0] == ' ')
	{
		reader->start++;
		if (!map_reader_fill_buffer(reader, MAX_ENTRY_LEN))
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
		return (map_reader_parse_entry(reader, entry));
	return (true);
}
