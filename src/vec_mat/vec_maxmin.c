/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_maxmin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:04:20 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:47:51 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec_mat.h"

#include <math.h>

t_vec4	max4(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){
		.v[0] = fmaxf(a.v[0], b.v[0]),
		.v[1] = fmaxf(a.v[1], b.v[1]),
		.v[2] = fmaxf(a.v[2], b.v[2]),
		.v[3] = fmaxf(a.v[3], b.v[3]),
	});
}

t_vec4	min4(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){
		.v[0] = fminf(a.v[0], b.v[0]),
		.v[1] = fminf(a.v[1], b.v[1]),
		.v[2] = fminf(a.v[2], b.v[2]),
		.v[3] = fminf(a.v[3], b.v[3]),
	});
}
