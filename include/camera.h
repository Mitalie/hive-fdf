/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:52:49 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 17:15:02 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "vec_mat.h"

typedef struct s_camera
{
	float	azimuth_deg;
	float	elevation_deg;
	float	aspect_ratio;
}	t_camera;

t_mat4	camera_transformation(t_camera *camera);

#endif
