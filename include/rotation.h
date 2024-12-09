/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:51:40 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 16:24:16 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROTATION_H
# define ROTATION_H

# include "vec_mat.h"

t_mat4	rotation_x(float angle_deg);
t_mat4	rotation_y(float angle_deg);
t_mat4	rotation_z(float angle_deg);

#endif
