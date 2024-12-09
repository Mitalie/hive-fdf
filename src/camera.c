/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:02:26 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 17:28:30 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include <math.h>
#include "rotation.h"

/*
	- translate camera position (10,10,10) to origin
	- rotate scene around camera y axis
	- rotate scene around camera x axis
	- perspective projection: scale view x and y bounds to [-1,1],
	  put -z into w for perspective divide, 1 into z to store depth as -1/z
*/
t_mat4	camera_transformation(t_camera *camera)
{
	t_mat4	transform;
	t_mat4	next;

	transform = mat4(
			vec4(1, 0, 0, -10),
			vec4(0, 1, 0, -10),
			vec4(0, 0, 1, -10),
			vec4(0, 0, 0, 1));
	next = rotation_y(-camera->azimuth_deg);
	transform = mul_mm4(&next, &transform);
	next = rotation_x(-camera->elevation_deg);
	transform = mul_mm4(&next, &transform);
	next = mat4(
			vec4(2 / camera->aspect_ratio, 0, 0, 0),
			vec4(0, 2, 0, 0),
			vec4(0, 0, 0, 1),
			vec4(0, 0, -1, 0));
	transform = mul_mm4(&next, &transform);
	return (transform);
}
