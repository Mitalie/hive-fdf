/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:52:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 17:42:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "vec_mat.h"

/*
	Azimuth and elevation are anticlockwise rotations of the camera. When
	transforming world into view space, the rotations need to be reversed.
*/
typedef struct s_camera
{
	float	azimuth_deg;
	float	elevation_deg;
	float	aspect_ratio;
}	t_camera;

t_mat4	camera_transformation(t_camera *camera);

#endif
