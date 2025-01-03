/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:48:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/16 22:06:20 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

static t_vec4	row(t_mat4 m, int row)
{
	return ((t_vec4){
		.v[0] = m.col[0].v[row],
		.v[1] = m.col[1].v[row],
		.v[2] = m.col[2].v[row],
		.v[3] = m.col[3].v[row],
	});
}

/*
	With -O2 -flto optimization, the compiler inlines these calls and computes
	each row's dot product in parallel with SIMD instructions. Thanks to column
	major order, each column can be read and multiplied in one go without having
	to shuffle elements around.
*/

t_vec4	mul_mv4(t_mat4 m, t_vec4 v)
{
	return ((t_vec4){
		.v[0] = dot4(row(m, 0), v),
		.v[1] = dot4(row(m, 1), v),
		.v[2] = dot4(row(m, 2), v),
		.v[3] = dot4(row(m, 3), v),
	});
}

t_mat4	mul_mm4(t_mat4 a, t_mat4 b)
{
	return ((t_mat4){
		.col[0] = mul_mv4(a, b.col[0]),
		.col[1] = mul_mv4(a, b.col[1]),
		.col[2] = mul_mv4(a, b.col[2]),
		.col[3] = mul_mv4(a, b.col[3]),
	});
}

t_mat4	transpose4(t_mat4 m)
{
	return ((t_mat4){
		.col[0] = row(m, 0),
		.col[1] = row(m, 1),
		.col[2] = row(m, 2),
		.col[3] = row(m, 3),
	});
}
