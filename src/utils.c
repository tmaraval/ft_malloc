/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaraval <tmaraval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 14:51:56 by tmaraval          #+#    #+#             */
/*   Updated: 2019/09/25 14:54:10 by tmaraval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

// Take a zone pointer (tiny/medium)
// And convert it to start of usable memory
void *zone_2_mem(t_zone *zone)
{
	return ((void *)zone + sizeof(t_zone));
}