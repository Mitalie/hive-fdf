/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:43:05 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/26 15:03:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_READER_H
# define FILE_READER_H

# include <stdbool.h>
# include <stddef.h>

typedef struct s_file_reader
{
	size_t	buf_size;
	char	*buf;
	char	*start;
	char	*end;
	int		fd;
	bool	eof;
}	t_file_reader;

bool	file_reader_open(
			t_file_reader *reader, size_t buf_size, const char *filename);
bool	file_reader_close(t_file_reader *reader);
bool	file_reader_fill_buffer(t_file_reader *reader, size_t min_length);

#endif
