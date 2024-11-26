/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:52:16 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/26 15:57:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_INTERNAL_H
# define MAP_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include "buffer.h"
# include "file_reader.h"
# include "mesh.h"

/*
	Entries can be parsed from file one at a time without backtracking, using
	file_reader to load data into parsing buffer.
*/
typedef struct s_map_entry
{
	int32_t		z;
	uint32_t	color;
	bool		is_end;
	bool		is_newline;
}	t_map_entry;

/*
	Map builder accumulates entries, keeping track of X and Y positions and
	automatically connecting the vertices with lines along the grid. When done,
	it centers the model on origin and converts the data into t_mesh.
*/
typedef struct s_map_builder
{
	t_buffer	vertices;
	t_buffer	lines;
	uint32_t	y;
	uint32_t	x;
	uint32_t	last_x;
	uint32_t	max_x;
	uint32_t	n_vertices;
	uint32_t	n_lines;
}	t_map_builder;

void	map_builder_init(t_map_builder *builder);
void	map_builder_release(t_map_builder *builder);
bool	map_builder_add_entry(t_map_builder *builder, t_map_entry *entry);
bool	map_builder_finalize(t_map_builder *builder, t_mesh *mesh);

#endif
