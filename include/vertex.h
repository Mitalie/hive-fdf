/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:18:28 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:51:08 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEX_H
# define VERTEX_H

# include <stdint.h>

# include "vec_mat.h"

typedef struct s_vertex
{
	t_vec4		pos;
	uint32_t	color;
}	t_vertex;

#endif
