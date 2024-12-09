/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:59:07 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 20:35:28 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

t_vec4	add4(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){
		.v[0] = a.v[0] + b.v[0],
		.v[1] = a.v[1] + b.v[1],
		.v[2] = a.v[2] + b.v[2],
		.v[3] = a.v[3] + b.v[3],
	});
}

t_vec4	sub4(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){
		.v[0] = a.v[0] - b.v[0],
		.v[1] = a.v[1] - b.v[1],
		.v[2] = a.v[2] - b.v[2],
		.v[3] = a.v[3] - b.v[3],
	});
}

t_vec4	mul_sv4(float s, t_vec4 v)
{
	return ((t_vec4){
		.v[0] = s * v.v[0],
		.v[1] = s * v.v[1],
		.v[2] = s * v.v[2],
		.v[3] = s * v.v[3],
	});
}

/*
	dot4 implementation is located in mat.c for optimization reasons
*/
