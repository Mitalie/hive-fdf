/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:48:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/04 17:52:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

float	dot4(t_vec4 a, t_vec4 b)
{
	return (
		+ a.v[0] * b.v[0]
		+ a.v[1] * b.v[1]
		+ a.v[2] * b.v[2]
		+ a.v[3] * b.v[3]
	);
}

static t_vec4	row(const t_mat4 *m, int row)
{
	return ((t_vec4){
		.v[0] = m->col[0].v[row],
		.v[1] = m->col[1].v[row],
		.v[2] = m->col[2].v[row],
		.v[3] = m->col[3].v[row],
	});
}

/*
	With -O2 optimization, the compiler inlines these calls and computes each
	row's dot product in parallel with SIMD instructions. Thanks to column major
	order, each column can be read and multiplied in one go without having to
	shuffle elements around.
*/

t_vec4	mul_mv4(const t_mat4 *m, t_vec4 v)
{
	return ((t_vec4){
		.v[0] = dot4(row(m, 0), v),
		.v[1] = dot4(row(m, 1), v),
		.v[2] = dot4(row(m, 2), v),
		.v[3] = dot4(row(m, 3), v),
	});
}

t_mat4	mul_mm4(const t_mat4 *a, const t_mat4 *b)
{
	return ((t_mat4){
		.col[0] = mul_mv4(a, b->col[0]),
		.col[1] = mul_mv4(a, b->col[1]),
		.col[2] = mul_mv4(a, b->col[2]),
		.col[3] = mul_mv4(a, b->col[3]),
	});
}

t_mat4	transpose4(const t_mat4 *m)
{
	return ((t_mat4){
		.col[0] = row(m, 0),
		.col[1] = row(m, 1),
		.col[2] = row(m, 2),
		.col[3] = row(m, 3),
	});
}
