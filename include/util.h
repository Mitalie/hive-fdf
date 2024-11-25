/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:09:01 by amakinen          #+#    #+#             */
/*   Updated: 2024/11/25 18:07:29 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

bool	parse_int32(
			const char *str, size_t len, size_t *parsed_len, int32_t *value);
bool	parse_uint32_hex(
			const char *str, size_t len, size_t *parsed_len, uint32_t *value);

#endif
