/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 16:27:21 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/28 16:34:16 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "projection.h"
#include <math.h>

t_mat4f	projection_ortho(t_ortho_bounds bounds, float yaw, float pitch)
{
	t_mat4f	transform;
	t_mat4f	next;

	transform = (t_mat4f){
		(t_vec4f){{cosf(yaw), 0, sinf(yaw), 0}},
		(t_vec4f){{0, 1, 0, 0}},
		(t_vec4f){{-sinf(yaw), 0, cosf(yaw), 0}},
		(t_vec4f){{0, 0, 0, 1}},
	};
	next = (t_mat4f){
		(t_vec4f){{1, 0, 0, 0}},
		(t_vec4f){{0, cosf(pitch), sinf(pitch), 0}},
		(t_vec4f){{0, -sinf(pitch), cosf(pitch), 0}},
		(t_vec4f){{0, 0, 0, 1}},
	};
	transform = mul4f_mat_mat(&next, &transform);
	next = (t_mat4f){
		(t_vec4f){{2 / (bounds.right - bounds.left), 0, 0, 0}},
		(t_vec4f){{0, 2 / (bounds.top - bounds.bottom), 0, 0}},
		(t_vec4f){{0, 0, 1, 0}},
		(t_vec4f){{-(bounds.right + bounds.left) / (bounds.right - bounds.left),  -(bounds.top + bounds.bottom) / (bounds.top - bounds.bottom), 0, 1}},
	};
	return (mul4f_mat_mat(&next, &transform));
}
