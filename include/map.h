/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:45:09 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/18 20:13:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include <stdint.h>
# include "MLX42/MLX42.h"

typedef struct s_map
{
	uint32_t	size_x;
	uint32_t	size_y;
	int32_t		*z;
	uint32_t	*color;
}	t_map;

void	draw_map(mlx_image_t *image, t_map *map);

#endif
