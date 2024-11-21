/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:18:28 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/21 18:21:13 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEX_H
# define VERTEX_H

# include <stdint.h>
# include "vec_mat.h"

typedef struct s_vertex
{
	t_vec4f		pos;
	uint32_t	color;
}	t_vertex;


#endif
