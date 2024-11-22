/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:48:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/22 17:44:33 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

float	dot4f(t_vec4f a, t_vec4f b)
{
	return (
		+ a.v[0] * b.v[0]
		+ a.v[1] * b.v[1]
		+ a.v[2] * b.v[2]
		+ a.v[3] * b.v[3]
	);
}

static t_vec4f	row(const t_mat4f *m, int row)
{
	return ((t_vec4f){
		{
			m->col[0].v[row],
			m->col[1].v[row],
			m->col[2].v[row],
			m->col[3].v[row],
		}
	});
}

/*
	With -O2 optimization, the compiler inlines these calls and computes each
	row's dot product in parallel with SIMD instructions. Thanks to column major
	order, each column can be read and multiplied in one go without having to
	shuffle elements around.
*/

t_vec4f	mul4f_mat_vec(const t_mat4f *m, t_vec4f v)
{
	return ((t_vec4f){
		{
			dot4f(row(m, 0), v),
			dot4f(row(m, 1), v),
			dot4f(row(m, 2), v),
			dot4f(row(m, 3), v),
		}
	});
}

t_mat4f	mul4f_mat_mat(const t_mat4f *a, const t_mat4f *b)
{
	return ((t_mat4f){
		{
			mul4f_mat_vec(a, b->col[0]),
			mul4f_mat_vec(a, b->col[1]),
			mul4f_mat_vec(a, b->col[2]),
			mul4f_mat_vec(a, b->col[3]),
		}
	});
}
