/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:52:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 18:06:11 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include <stdbool.h>
# include "vec_mat.h"

/*
	Azimuth and elevation are anticlockwise rotations of the camera. When
	transforming world into view space, the rotations need to be reversed.
*/
typedef struct s_camera
{
	t_vec4	position;
	float	azimuth_deg;
	float	elevation_deg;
	float	aspect_ratio;
	bool	perspective;
	bool	move_angled;
}	t_camera;

void	camera_reset(t_camera *camera);
void	camera_move(t_camera *camera, float right, float up, float back);
t_mat4	camera_transformation(t_camera *camera);

#endif