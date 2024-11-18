/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/18 14:23:59 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_MAT_H
# define VEC_MAT_H

/*
	The union allows using either named xyzw fields or indices to access same
	data.
*/

typedef union u_vec4f
{
	struct {
		float	v[4];
	};
	struct {
		float	x;
		float	y;
		float	z;
		float	w;
	};
}	t_vec4f;

/*
	Matrices are stored in column-major order to allow optimizing compiler to
	generate more efficient SIMD instructions for matrix-vector multiplication.
*/

typedef struct s_mat4f
{
	t_vec4f	col[4];
}	t_mat4f;

float	dot4f(t_vec4f a, t_vec4f b);
t_vec4f	mul4f_mat_vec(t_mat4f m, t_vec4f v);
t_mat4f	mul4f_mat_mat(t_mat4f a, t_mat4f b);

#endif
