/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 20:58:08 by amakinen          #+#    #+#             */
/*   Updated: 2024/12/19 21:06:54 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_util.h"

#include <stdint.h>
#include <stdlib.h>

char	*str_join(const char *str1, const char *str2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;
	char	*joined_write;

	len1 = 0;
	while (str1[len1])
		len1++;
	len2 = 0;
	while (str2[len2])
		len2++;
	if (len1 > SIZE_MAX - 1 || len2 > SIZE_MAX - 1 - len1)
		return (0);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (0);
	joined_write = joined;
	while (*str1)
		*joined_write++ = *str1++;
	while (*str2)
		*joined_write++ = *str2++;
	*joined_write = 0;
	return (joined);
}
