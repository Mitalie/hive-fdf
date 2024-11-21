/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:45:50 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/21 16:07:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_H
# define BUFFER_H

# include <stddef.h>
# include <stdbool.h>

typedef struct s_buffer
{
	size_t	len;
	size_t	size;
	char	*buf;
}	t_buffer;

void	buffer_init(t_buffer *b);
void	buffer_free(t_buffer *b);
bool	buffer_grow(t_buffer *b, size_t min_space);
bool	buffer_append(t_buffer *b, void *data, size_t len);

#endif
