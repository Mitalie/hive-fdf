/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:02:26 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 18:09:17 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include <math.h>
#include "rotation.h"

void	camera_reset(t_camera *camera)
{
	camera->position = vec4(10, 10, 10, 1);
	camera->azimuth_deg = 45;
	camera->elevation_deg = -35.2643897f;
	camera->perspective = false;
	camera->move_angled = false;
}

void	camera_move(t_camera *camera, float right, float up, float back)
{
	t_mat4	transform;
	t_mat4	next;
	t_vec4	movement;

	if (camera->perspective && camera->move_angled)
	{
		transform = rotation_x(camera->elevation_deg);
		next = rotation_y(camera->azimuth_deg);
		transform = mul_mm4(&next, &transform);
	}
	else
		transform = rotation_y(camera->azimuth_deg);
	movement = mul_mv4(&transform, vec4(right, up, back, 0));
	camera->position = add4(camera->position, movement);
}

static t_mat4	camera_projection_perspective(t_camera *camera)
{
	t_mat4	transform;

	transform = mat4(
			vec4(2 / camera->aspect_ratio, 0, 0, 0),
			vec4(0, 2, 0, 0),
			vec4(0, 0, 0, 1),
			vec4(0, 0, -1, 0));
	return (transform);
}

static t_mat4	camera_projection_orthograpic(t_camera *camera)
{
	t_mat4	transform;

	transform = mat4(
			vec4(0.1f / camera->aspect_ratio, 0, 0, 0),
			vec4(0, 0.1f, 0, 0),
			vec4(0, 0, 1, 0),
			vec4(0, 0, 0, 1));
	return (transform);
}

/*
	- translate camera position to origin
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
			vec4(1, 0, 0, -camera->position.x),
			vec4(0, 1, 0, -camera->position.y),
			vec4(0, 0, 1, -camera->position.z),
			vec4(0, 0, 0, 1));
	next = rotation_y(-camera->azimuth_deg);
	transform = mul_mm4(&next, &transform);
	next = rotation_x(-camera->elevation_deg);
	transform = mul_mm4(&next, &transform);
	if (camera->perspective)
		next = camera_projection_perspective(camera);
	else
		next = camera_projection_orthograpic(camera);
	transform = mul_mm4(&next, &transform);
	return (transform);
}