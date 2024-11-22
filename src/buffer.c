/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:47:00 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 17:43:20 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buffer.h"
#include <stdlib.h>
#include <stdint.h>

void	buffer_init(t_buffer *b)
{
	b->len = 0;
	b->size = 0;
	b->buf = 0;
}

void	buffer_release(t_buffer *b)
{
	free(b->buf);
	buffer_init(b);
}

// Reasonable minimum avoids very small reallocs without taking too much memory
#define MIN_SIZE 256

static bool	buffer_ensure_space(t_buffer *b, size_t min_space)
{
	size_t	new_size;
	char	*new_buf;
	size_t	i;

	if (min_space <= b->size - b->len)
		return (true);
	if (min_space > SIZE_MAX - b->len)
		return (false);
	new_size = b->size;
	if (new_size == 0)
		new_size = MIN_SIZE;
	while (new_size < min_space + b->len && new_size < SIZE_MAX / 2)
		new_size *= 2;
	if (new_size < min_space + b->len)
		new_size = SIZE_MAX;
	new_buf = malloc(new_size);
	if (!new_buf)
		return (false);
	i = -1;
	while (++i < b->len)
		new_buf[i] = b->buf[i];
	free(b->buf);
	b->buf = new_buf;
	b->size = new_size;
	return (true);
}

bool	buffer_append(t_buffer *b, void *data, size_t len)
{
	size_t	i;

	if (!buffer_ensure_space(b, len))
		return (false);
	i = -1;
	while (++i < len)
		b->buf[i + b->len] = ((char *)data)[i];
	b->len += len;
	return (true);
}

void	*buffer_finalize(t_buffer *b)
{
	char	*new_buf;
	size_t	i;

	new_buf = malloc(b->len);
	if (!new_buf)
		return (0);
	i = -1;
	while (++i < b->len)
		new_buf[i] = b->buf[i];
	buffer_release(b);
	return (new_buf);
}
