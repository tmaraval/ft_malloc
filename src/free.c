/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaraval <tmaraval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 11:30:28 by tmaraval          #+#    #+#             */
/*   Updated: 2019/10/11 13:57:16 by tmaraval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	munmap_large(t_zone **zone, void *ptr)
{
	t_zone *del;

	del = ptr - sizeof(t_zone);
	delete_zone(zone, del, aligne_large(del->size + sizeof(t_zone)));
}

void	munmap_small_medium(t_zone **zones)
{
	t_zone	*zone;
	int		i;

	i = 0;
	zone = *zones;
	while (zone != NULL)
	{
	//	shrink_zone(&zone);
		page_free(&zone);
		if (should_delete_zone(zone))
		{
			delete_zone(zones, zone, getpagesize());
			zone = *zones;
			i = 0;
		}
		else
		{
			zone = zone->next;
			i++;
		}

	}
}

void	ft_free(void *ptr)
{
	t_chunk *chunk;

	if (ptr == NULL)
		return ;
	if (search_chunk(&(g_malloc_state.zone_tiny), ptr) == 1)
	{
		chunk = ptr - sizeof(t_chunk);
		chunk->status = FREE;
		munmap_small_medium(&(g_malloc_state.zone_tiny));
	}
	else if (search_chunk(&(g_malloc_state.zone_medium), ptr) == 1)
	{
		chunk = ptr - sizeof(t_chunk);
		chunk->status = FREE;
		munmap_small_medium(&(g_malloc_state.zone_medium));
	}
	else if (search_chunk_large(&g_malloc_state.zone_large, ptr) == 1)
	{
		munmap_large(&g_malloc_state.zone_large, ptr);
	}
	else
		;
#ifdef DEBUG_STATE
	debug_all(&g_malloc_state);
#endif
	return ;


#ifdef DEBUG_FREE
	ft_putendl("|DEBUG| -> after free");
	print_chunks(g_malloc_state.zone_tiny->chunks, "chunk_tiny");
#endif
}
