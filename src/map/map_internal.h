/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:52:16 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 18:19:11 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_INTERNAL_H
# define MAP_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include "buffer.h"
# include "mesh.h"

typedef struct s_map_entry
{
	int32_t		z;
	uint32_t	color;
	bool		is_end;
	bool		is_invalid;
	bool		is_newline;
	bool		has_color;
}	t_map_entry;

# define READ_BUF_SIZE 1024

typedef struct s_map_reader
{
	size_t	start;
	size_t	len;
	int		fd;
	char	buf[READ_BUF_SIZE];
}	t_map_reader;

bool	map_reader_open(t_map_reader *reader, const char *filename);
void	map_reader_close(t_map_reader *reader);
bool	map_reader_next_entry(t_map_reader *reader, t_map_entry *entry);

typedef struct s_map_builder
{
	t_buffer	vertices;
	t_buffer	lines;
	uint32_t	y;
	uint32_t	x;
	uint32_t	last_x;
	uint32_t	n_vertices;
	uint32_t	n_lines;
}	t_map_builder;

void	map_builder_init(t_map_builder *builder);
void	map_builder_release(t_map_builder *builder);
bool	map_builder_add_entry(t_map_builder *builder, t_map_entry *entry);
bool	map_builder_finalize(t_map_builder *builder, t_mesh *mesh);

#endif
