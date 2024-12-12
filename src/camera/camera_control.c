/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:48:24 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/12 20:13:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "rotation.h"

void	camera_reset(t_camera *camera)
{
	camera->position = vec4(10, 10, 10, 1);
	camera->azimuth_deg = 45;
	camera->elevation_deg = -35.2643897f;
	camera->zoom_exp = 0;
	camera->perspective = false;
	camera->move_angled = false;
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
		transform = mul_mm4(&transform, &transform_pre);
	}
	movement = mul_mv4(&transform, movement);
	camera->position = add4(camera->position, movement);
}
