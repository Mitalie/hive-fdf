/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:39:29 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/11 14:31:58 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_INTERNAL_H
# define LINE_INTERNAL_H

# include "vertex.h"

typedef struct s_line_data
{
	t_vertex	a;
	t_vertex	b;
	float		ta;
	float		tb;
}	t_line_data;

#endif
