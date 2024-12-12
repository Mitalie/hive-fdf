/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:00:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/12 21:12:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_INTERNAL_H
# define INPUT_INTERNAL_H

# include "MLX42/MLX42.h"
# include "fdf.h"

typedef void (t_input_timed_func)(t_fdf *fdf, int arg, float time);

typedef struct s_input_timed
{
	keys_t				key;
	t_input_timed_func	*func;
	int					arg;
}	t_input_timed;

#endif
