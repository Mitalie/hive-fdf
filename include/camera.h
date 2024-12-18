/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:52:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/18 18:17:13 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include <stdbool.h>
# include "vec_mat.h"

/*
	Flying can move up/down with forward movement depending on camera elevation
	angle. Floating separates horizontal and vertical controls.
*/
typedef enum e_camera_mode
{
	CAM_ORTHO,
	CAM_PERSP_FLYING,
	CAM_PERSP_FLOATING,
	NUM_CAM_MODES,
}	t_camera_mode;

/*
	Azimuth and elevation are anticlockwise rotations of the camera. When
	transforming world into view space, the rotations need to be reversed.
*/
typedef struct s_camera
{
	t_vec4			position;
	float			azimuth_deg;
	float			elevation_deg;
	float			aspect_ratio;
	float			zoom_exp;
	t_camera_mode	mode;
}	t_camera;

typedef enum e_camera_dir
{
	CAM_RIGHT,
	CAM_LEFT,
	CAM_UP,
	CAM_DOWN,
	CAM_BACK,
	CAM_FRONT,
}	t_camera_dir;

void	camera_reset(t_camera *camera);
void	camera_next_mode(t_camera *camera);
void	camera_fit_box(t_camera *camera, t_vec4 box_min, t_vec4 box_max);
void	camera_move(t_camera *camera, t_camera_dir dir, float amount);
void	camera_rotate(t_camera *camera, t_camera_dir dir, float amount);
t_mat4	camera_transformation(t_camera *camera);

#endif
