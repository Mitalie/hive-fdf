/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/02 16:33:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_MAT_H
# define VEC_MAT_H

/*
	The union allows using either named xyzw fields or indices to access same
	data.

	At 16 bytes, this struct is small enough that it can be passed and returned
	by value in registers on x84-64 System V ABI.
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

	At 64 bytes, this struct should probably be passed by reference to prevent
	unnecessary copying.
*/

typedef struct s_mat4f
{
	t_vec4f	col[4];
}	t_mat4f;

float	dot4f(t_vec4f a, t_vec4f b);
t_vec4f	mul4f_mat_vec(const t_mat4f *m, t_vec4f v);
t_mat4f	mul4f_mat_mat(const t_mat4f *a, const t_mat4f *b);
t_mat4f	transpose4f(const t_mat4f *m);

t_vec4f	vec4f(float x, float y, float z, float w);
/*
	Create a matrix by using each vector as a row of the matrix, allowing source
	code layout to match the logical layout of the matrix.
*/
t_mat4f	mat4f(t_vec4f row0, t_vec4f row1, t_vec4f row2, t_vec4f row3);

#endif
