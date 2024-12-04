/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:10:58 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/04 17:50:17 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

t_vec4	vec4(float x, float y, float z, float w)
{
	return ((t_vec4){
		.x = x,
		.y = y,
		.z = z,
		.w = w,
	});
}

t_mat4	mat4(t_vec4 row0, t_vec4 row1, t_vec4 row2, t_vec4 row3)
{
	t_mat4	m;

	m = (t_mat4){
		.col[0] = row0,
		.col[1] = row1,
		.col[2] = row2,
		.col[3] = row3,
	};
	return (transpose4(&m));
}
