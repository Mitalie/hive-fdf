/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_transform.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:02:26 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:48:43 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

#include <math.h>

#include "rotation.h"
#include "vec_mat.h"

/*
	For perspective projection, without a near clip plane in front of the camera
	lines that pass exactly through the camera (or close enough with rounding)
	get a clipped w coordinate of zero, which sets all coordinates to infinity
	in perspective division and breaks drawing. To avoid geometry noticeably
	disappearing, we set the plane very close.
*/
#define NEAR_PLANE_DIST 0.001f

/*
	- store distance from camera (-z) in w for perspective divide
	- scale view x and y bounds to -w <= x <= w and -w <= y <= w
	- store near plane distance in z to clip objects too close to camera (z <= w)
*/
static t_mat4	camera_projection_perspective(t_camera *camera)
{
	t_mat4	transform;
	float	y_scale;
	float	x_scale;

	y_scale = powf(2, camera->zoom_exp);
	x_scale = y_scale / camera->aspect_ratio;
	transform = mat4(
			vec4(x_scale, 0, 0, 0),
			vec4(0, y_scale, 0, 0),
			vec4(0, 0, 0, NEAR_PLANE_DIST),
			vec4(0, 0, -1, 0));
	return (transform);
}

/*
	- put 1 into w to make perspective divide a no-op
	- scale view x and y bounds to -1 <= x <= 1 and -1 <= y <= 1
	- offset z to move camera to near clip plane (z <= 1)

	Orthogonal projection could work without near clip plane if we scale z to
	stay below 1, but being able to "peel" away geometry and seeing what's
	behind it is cool.
*/

static t_mat4	camera_projection_orthograpic(t_camera *camera)
{
	t_mat4	transform;
	float	y_scale;
	float	x_scale;

	y_scale = powf(2, camera->zoom_exp);
	x_scale = y_scale / camera->aspect_ratio;
	transform = mat4(
			vec4(x_scale, 0, 0, 0),
			vec4(0, y_scale, 0, 0),
			vec4(0, 0, 1, 1),
			vec4(0, 0, 0, 1));
	return (transform);
}

/*
	- translate camera position to origin
	- rotate scene around camera y axis
	- rotate scene around camera x axis
	- transform to clip space according to the projection
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
	transform = mul_mm4(next, transform);
	next = rotation_x(-camera->elevation_deg);
	transform = mul_mm4(next, transform);
	if (camera->mode == CAM_ORTHO)
		next = camera_projection_orthograpic(camera);
	else
		next = camera_projection_perspective(camera);
	transform = mul_mm4(next, transform);
	return (transform);
}
