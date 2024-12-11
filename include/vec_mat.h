/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mat.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/11 14:56:55 by amakinen         ###   ########.fr       */
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

typedef union u_vec4
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
}	t_vec4;

t_vec4	add4(t_vec4 a, t_vec4 b);
t_vec4	sub4(t_vec4 a, t_vec4 b);
t_vec4	mul_sv4(float s, t_vec4 v);
t_vec4	lerp4(t_vec4 a, t_vec4 b, float t);
float	dot4(t_vec4 a, t_vec4 b);

/*
	Matrices are stored in column-major order to allow optimizing compiler to
	generate more efficient SIMD instructions for matrix-vector multiplication.

	At 64 bytes, this struct should probably be passed by reference to prevent
	unnecessary copying.
*/

typedef struct s_mat4
{
	t_vec4	col[4];
}	t_mat4;

t_vec4	mul_mv4(const t_mat4 *m, t_vec4 v);
t_mat4	mul_mm4(const t_mat4 *a, const t_mat4 *b);
t_mat4	transpose4(const t_mat4 *m);

/*
	Helper functions to make creating matrices in code slightly more ergonomic.
*/

t_vec4	vec4(float x, float y, float z, float w);
/*
	Create a matrix by using each vector as a row of the matrix, allowing source
	code layout to match the logical layout of the matrix.
*/
t_mat4	mat4(t_vec4 row0, t_vec4 row1, t_vec4 row2, t_vec4 row3);

#endif
