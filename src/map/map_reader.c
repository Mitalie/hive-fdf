/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:16:09 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 19:43:47 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h> // strtol - replace
#include <unistd.h>

bool	map_reader_open(t_map_reader *reader, const char *filename)
{
	reader->start = 0;
	reader->len = 0;
	reader->fd = open(filename, O_RDONLY);
	reader->buf[READ_BUF_SIZE] = 0;
	return (reader->fd >= 0);
}

void	map_reader_close(t_map_reader *reader)
{
	// TODO errors?
	close(reader->fd);
}

static bool	map_reader_fill_buffer(t_map_reader *reader, size_t min_length)
{
	size_t	i;
	ssize_t	ret;

	if (reader->len >= min_length)
		return (true);
	i = 0;
	while (i < reader->len)
	{
		reader->buf[i] = reader->buf[reader->start + i];
		i++;
	}
	reader->start = 0;
	while (reader->len < min_length)
	{
		ret = read(reader->fd, reader->buf + reader->len,
			sizeof(reader->buf) - reader->len);
		if (ret < 0 && errno == EINTR)
			continue ;
		if (ret < 0)
			return (false);
		if (ret == 0)
			break ;
		reader->len += ret;
	}
	return (true);
}

// "-2147483648,0xffffff" is 20 characters, add one for space/newline
#define MAX_ENTRY_LEN 21

bool	map_reader_next_entry(t_map_reader *reader, t_map_entry *entry)
{
	long	parsed;
	char	*end;

	entry->is_end = false;
	entry->is_newline = false;
	if (!map_reader_fill_buffer(reader, MAX_ENTRY_LEN))
		return (false);
	while (reader->len > 0 && reader->buf[reader->start] == ' ')
	{
		reader->start++;
		reader->len--;
		if (!map_reader_fill_buffer(reader, MAX_ENTRY_LEN))
			return (false);
	}
	if (reader->len == 0)
		entry->is_end = true;
	else if (reader->buf[reader->start] == '\n')
	{
		reader->start++;
		reader->len--;
		entry->is_newline = true;
	}
	else
	{
		parsed = strtol(reader->buf + reader->start, &end, 10);
		if (parsed < INT32_MIN || parsed > INT32_MAX || (end < reader->buf + reader->start + reader->len && *end != ' ' && *end != '\n' && *end != ','))
			return (false);
		entry->z = parsed;
		if (end >= reader->buf + reader->start + reader->len || *end != ',')
			entry->color = 0xffffffff;
		else
		{
			parsed = strtol(end + 1, &end, 16);
			if (parsed < 0 || parsed > 1 << 24 || (end < reader->buf + reader->start + reader->len && *end != ' ' && *end != '\n'))
				return (false);
			entry->color = parsed << 8 | 0xff;
		}
		if (end < reader->buf + reader->start + reader->len)
		{
			reader->len -= end - reader->buf - reader->start;
			reader->start += end - reader->buf - reader->start;
		}
		else
			reader->len = 0;
	}
	return (true);
}
