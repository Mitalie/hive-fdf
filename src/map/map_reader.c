/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:16:09 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 18:19:20 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"
#include <fcntl.h>
#include <unistd.h>

bool	map_reader_open(t_map_reader *reader, const char *filename)
{
	reader->start = 0;
	reader->len = 0;
	reader->fd = open(filename, O_RDONLY);
	return (reader->fd >= 0);
}

void	map_reader_close(t_map_reader *reader)
{
	// TODO errors?
	close(reader->fd);
}

bool	map_reader_next_entry(t_map_reader *reader, t_map_entry *entry)
{
	while (reader->buf[reader->start] == ' ')
		reader->start++;
	if (reader->buf[reader->start] == '\n')
		entry->is_newline = true;
}
