/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:52:40 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:47:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rotation.h"

#include <math.h>

#include "vec_mat.h"

t_mat4	rotation_x(float angle_deg)
{
	float	angle_rad;
	t_mat4	transform;

	angle_rad = angle_deg / 180.0f * 3.1415926535f;
	transform = mat4(
			vec4(1, 0, 0, 0),
			vec4(0, cosf(angle_rad), -sinf(angle_rad), 0),
			vec4(0, sinf(angle_rad), cosf(angle_rad), 0),
			vec4(0, 0, 0, 1));
	return (transform);
}

t_mat4	rotation_y(float angle_deg)
{
	float	angle_rad;
	t_mat4	transform;

	angle_rad = angle_deg / 180.0f * 3.1415926535f;
	transform = mat4(
			vec4(cosf(angle_rad), 0, sinf(angle_rad), 0),
			vec4(0, 1, 0, 0),
			vec4(-sinf(angle_rad), 0, cosf(angle_rad), 0),
			vec4(0, 0, 0, 1));
	return (transform);
}

t_mat4	rotation_z(float angle_deg)
{
	float	angle_rad;
	t_mat4	transform;

	angle_rad = angle_deg / 180.0f * 3.1415926535f;
	transform = mat4(
			vec4(cosf(angle_rad), -sinf(angle_rad), 0, 0),
			vec4(sinf(angle_rad), cosf(angle_rad), 0, 0),
			vec4(1, 0, 0, 0),
			vec4(0, 0, 0, 1));
	return (transform);
}
