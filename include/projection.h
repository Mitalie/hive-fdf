/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 16:23:06 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/28 16:35:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROJECTION_H
# define PROJECTION_H

# include "vec_mat.h"

typedef struct s_ortho_bounds {
	float	top;
	float	bottom;
	float	right;
	float	left;
}	t_ortho_bounds;

t_mat4f	projection_ortho(t_ortho_bounds bounds, float yaw, float pitch);

#endif
