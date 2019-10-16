/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaraval <tmaraval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 10:51:30 by tmaraval          #+#    #+#             */
/*   Updated: 2019/10/16 13:03:46 by tmaraval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	debug_munmap(void *addr, size_t size)
{
	if (getenv("DEBUG_MUNMAP"))
	{
		ft_putstr("|DEBUG| -> munmap(");
		ft_putptr(addr);
		ft_putstr(", ");
		ft_putnbr(size);
		ft_putendl(");");
	}
}
