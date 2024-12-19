/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:11:52 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 19:47:08 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

#include <stdbool.h>
#include <stdint.h>

bool	parse_int32(
	const char *str, size_t str_len, size_t *len, int32_t *value)
{
	size_t	skip;
	int32_t	sign;
	int32_t	digit;

	if (str_len == 0)
		return (false);
	skip = 0;
	sign = 1;
	if (str[0] == '-')
		sign = -1;
	if (str[0] == '-' || str[0] == '+')
		skip = 1;
	*len = skip;
	*value = 0;
	while (*len < str_len && str[*len] >= '0' && str[*len] <= '9')
	{
		digit = str[(*len)++] - '0';
		if ((sign > 0 && *value > (INT32_MAX - digit) / 10)
			|| (sign < 0 && *value < (INT32_MIN + digit) / 10))
			return (false);
		*value = *value * 10 + sign * digit;
	}
	return (*len > skip);
}

static bool	parse_hex_digit(const char c, uint32_t *parsed)
{
	if (c >= '0' && c <= '9')
		*parsed = (c - '0');
	else if (c >= 'a' && c <= 'f')
		*parsed = (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		*parsed = (c - 'A' + 10);
	else
		return (false);
	return (true);
}

bool	parse_uint32_hex(
	const char *str, size_t str_len, size_t *len, uint32_t *value)
{
	size_t		skip;
	uint32_t	digit;

	if (str_len == 0)
		return (false);
	skip = 0;
	if (str[0] == '+')
		skip = 1;
	if (str_len > skip + 1 && str[skip] == '0' && str[skip + 1] == 'x')
		skip += 2;
	*len = skip;
	*value = 0;
	while (*len < str_len && parse_hex_digit(str[*len], &digit))
	{
		(*len)++;
		if (*value > (UINT32_MAX - digit) / 16)
			return (false);
		*value = *value * 16 + digit;
	}
	return (*len > skip);
}
