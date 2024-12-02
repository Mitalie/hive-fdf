/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:10:58 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/02 15:59:26 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

t_vec4f	vec4f(float x, float y, float z, float w)
{
	return ((t_vec4f){
		.x = x,
		.y = y,
		.z = z,
		.w = w,
	});
}

t_mat4f	mat4f(t_vec4f row0, t_vec4f row1, t_vec4f row2, t_vec4f row3)
{
	t_mat4f	m;

	m = (t_mat4f){
		.col[0] = row0,
		.col[1] = row1,
		.col[2] = row2,
		.col[3] = row3,
	};
	return (transpose4f(&m));
}
