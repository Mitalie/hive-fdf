/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:48:24 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/18 18:26:43 by amakinen         ###   ########.fr       */
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
	camera->mode = CAM_ORTHO;
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
void	camera_next_mode(t_camera *camera)
{
	float			camera_dist;
	float			zoom_exp_delta;
	t_camera_mode	new_mode;

	new_mode = (camera->mode + 1) % NUM_CAM_MODES;
	camera->position.w = 0;
	camera_dist = sqrtf(dot4(camera->position, camera->position));
	zoom_exp_delta = log2f(camera_dist);
	if (new_mode == CAM_ORTHO)
		camera->zoom_exp -= zoom_exp_delta;
	if (camera->mode == CAM_ORTHO)
		camera->zoom_exp += zoom_exp_delta;
	camera->mode = new_mode;
}

/*
	Leave a tiny bit of space around the model when fitting to view.
*/
#define FIT_TARGET_SIZE 0.98f

/*
	Match view to a bounding box. Only makes sense with orthographic projection.
	The box must have been calculated with the current camera settings.
*/
void	camera_fit_box(t_camera *camera, t_vec4 box_min, t_vec4 box_max)
{
	t_vec4	size;
	t_vec4	center;
	float	scale;
	t_vec4	shift;
	t_mat4	transform;

	center = mul_sv4(0.5f, add4(box_min, box_max));
	scale = powf(2, -camera->zoom_exp);
	shift = vec4(
			center.x * scale * camera->aspect_ratio,
			center.y * scale,
			box_max.z,
			0);
	transform = mul_mm4(
			rotation_y(camera->azimuth_deg),
			rotation_x(camera->elevation_deg));
	shift = mul_mv4(transform, shift);
	camera->position = add4(camera->position, shift);
	size = sub4(box_max, box_min);
	camera->zoom_exp -= log2f(0.5f * fmaxf(size.x, size.y) / FIT_TARGET_SIZE);
}

/*
	Front/back are considered primary controls, while up/down are secondary.

	In orthographic projection, front/back move the camera in the view plane,
	and up/down adjust the near clip distance.

	In flying perspective projection, front/back move towards or away from what
	the camera is looking at, and up/down move in the view plane.

	In floating perspective projection, front/back are forced to be horizontal
	and up/down are forced to be vertical in the world space.
*/
void	camera_move(t_camera *camera, t_camera_dir dir, float amount)
{
	t_mat4	transform;
	t_vec4	movement;

	movement = vec4(
			amount * ((dir == CAM_RIGHT) - (dir == CAM_LEFT)),
			amount * ((dir == CAM_UP) - (dir == CAM_DOWN)),
			amount * ((dir == CAM_BACK) - (dir == CAM_FRONT)),
			0);
	transform = rotation_y(camera->azimuth_deg);
	if (camera->mode == CAM_PERSP_FLYING)
		transform = mul_mm4(transform, rotation_x(camera->elevation_deg));
	if (camera->mode == CAM_ORTHO)
		transform = mul_mm4(transform, rotation_x(camera->elevation_deg + 90));
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
