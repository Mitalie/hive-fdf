/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:00:25 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:48:31 by amakinen         ###   ########.fr       */
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

typedef void (t_input_event_func)(t_fdf *fdf, int arg);

typedef struct s_input_event
{
	keys_t				key;
	action_t			action;
	t_input_event_func	*func;
	int					arg;
}	t_input_event;

typedef enum e_input_dir
{
	INPUT_UP,
	INPUT_DOWN,
}	t_input_dir;

/*
	For merging simple argumentless actions in a single function...
*/
typedef enum e_input_simple_action
{
	INPUT_CLOSE,
	INPUT_RESET,
	INPUT_MODE,
	INPUT_FIT,
}	t_input_simple_action;

#endif
