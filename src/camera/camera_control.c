/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:48:24 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/17 15:40:45 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include <math.h>
#include "rotation.h"

void	camera_reset(t_camera *camera)
{
	camera->position = vec4(10, 10, 10, 0);
	camera->azimuth_deg = 45;
	camera->elevation_deg = -35.2643897f;
	camera->zoom_exp = 0;
	camera->perspective = false;
	camera->move_angled = false;
}

/*
	Toggle between perspective and orthographic projection.

	Attempt to adjust zoom level to keep the view similar, but we don't know
	actual distance to the part of the object in center of view, so we assume
	based on distance to plane that passes through origin.

	The position w coordinate needs to be zeroed or it incorrectly affects the
	distance calculation. It's not used anyway, but we don't have a separate
	vec3 type.
*/
void	camera_toggle_mode(t_camera *camera)
{
	float	camera_dist;
	float	zoom_exp_delta;

	camera->position.w = 0;
	camera_dist = sqrtf(dot4(camera->position, camera->position));
	zoom_exp_delta = log2f(camera_dist);
	if (camera->perspective)
		zoom_exp_delta = -zoom_exp_delta;
	camera->zoom_exp += zoom_exp_delta;
	camera->perspective = !camera->perspective;
}

void	camera_move(t_camera *camera, t_camera_dir dir, float amount)
{
	t_mat4	transform;
	t_mat4	transform_pre;
	t_vec4	movement;

	movement = vec4(
			amount * ((dir == CAM_RIGHT) - (dir == CAM_LEFT)),
			amount * ((dir == CAM_UP) - (dir == CAM_DOWN)),
			amount * ((dir == CAM_BACK) - (dir == CAM_FRONT)),
			0);
	transform = rotation_y(camera->azimuth_deg);
	if (camera->perspective && camera->move_angled)
	{
		transform_pre = rotation_x(camera->elevation_deg);
		transform = mul_mm4(transform, transform_pre);
	}
	movement = mul_mv4(transform, movement);
	camera->position = add4(camera->position, movement);
}

void	camera_rotate(t_camera *camera, t_camera_dir dir, float amount)
{
	camera->elevation_deg += amount * ((dir == CAM_UP) - (dir == CAM_DOWN));
	camera->azimuth_deg -= amount * ((dir == CAM_RIGHT) - (dir == CAM_LEFT));
	camera->elevation_deg = fmaxf(-90, fminf(90, camera->elevation_deg));
	camera->azimuth_deg = fmodf(camera->azimuth_deg, 360);
}
