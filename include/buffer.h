/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:45:50 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 17:34:58 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_H
# define BUFFER_H

# include <stddef.h>
# include <stdbool.h>

/*
	Growable buffer for collecting data before the final size is known.
*/
typedef struct s_buffer
{
	size_t	len;
	size_t	size;
	char	*buf;
}	t_buffer;

void	buffer_init(t_buffer *b);
void	buffer_release(t_buffer *b);
bool	buffer_append(t_buffer *b, void *data, size_t len);

/*
	Move all data in the buffer into an exactly sized allocation and release
	the buffer.
*/
void	*buffer_finalize(t_buffer *b);

#endif
