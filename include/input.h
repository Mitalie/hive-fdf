/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:59:28 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/18 18:36:07 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "fdf.h"

void	input_key(t_fdf *fdf, mlx_key_data_t key_data);
void	input_timed(t_fdf *fdf);

#endif
