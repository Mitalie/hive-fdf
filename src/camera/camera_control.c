/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:48:24 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/12 19:49:17 by amakinen         ###   ########.fr       */
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
