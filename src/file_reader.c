/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:45:51 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/11 17:03:41 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_reader.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

bool	file_reader_open(
	t_file_reader *reader, size_t buf_size, const char *filename)
{
	reader->buf_size = buf_size;
	reader->buf = malloc(buf_size);
	reader->start = reader->buf;
	reader->end = reader->buf;
	reader->fd = -1;
	reader->eof = false;
	if (!reader->buf)
		return (false);
	reader->fd = open(filename, O_RDONLY);
	return (reader->fd >= 0);
}

bool	file_reader_close(t_file_reader *reader)
{
	int	close_ret;

	free(reader->buf);
	close_ret = close(reader->fd);
	reader->buf_size = 0;
	reader->buf = 0;
	reader->start = 0;
	reader->end = 0;
	reader->fd = -1;
	reader->eof = true;
	return (close_ret == 0);
}

bool	file_reader_fill_buffer(t_file_reader *reader, size_t min_length)
{
	size_t	len;
	size_t	i;
	ssize_t	ret;

	len = reader->end - reader->start;
	if (len >= min_length || reader->eof)
		return (true);
	i = 0;
	while (i < len)
		reader->buf[i++] = *reader->start++;
	reader->start = reader->buf;
	reader->end = reader->start + len;
	while (!reader->eof && reader->end < reader->start + min_length)
	{
		ret = read(reader->fd, reader->end, reader->buf_size - len);
		if (ret < 0 && errno == EINTR)
			continue ;
		if (ret < 0)
			return (false);
		if (ret == 0)
			reader->eof = true;
		reader->end += ret;
		len += ret;
	}
	return (true);
}
