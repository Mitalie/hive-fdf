/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:02:27 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/17 17:06:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <math.h>
#include "camera.h"
#include "mesh.h"
#include "z_image.h"

void	fdf_draw(t_fdf *fdf)
{
	t_mat4	transform;
	float	height_scale;

	if (!fdf->need_redraw)
		return ;
	fdf->need_redraw = false;
	transform = camera_transformation(&fdf->camera);
	if (fdf->camera.perspective)
		z_image_clear(fdf->image, 0.0f, 0x000000ff);
	else
		z_image_clear(fdf->image, -INFINITY, 0x000000ff);
	height_scale = powf(2, fdf->height_scale_exp);
	fdf->mesh.scale.y = height_scale;
	draw_mesh(fdf->image, &fdf->mesh, transform);
}
