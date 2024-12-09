/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:02:44 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/09 20:33:29 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H

# include "z_image.h"
# include "vertex.h"

void	draw_line(t_z_image *image, t_vertex a, t_vertex b);

#endif
