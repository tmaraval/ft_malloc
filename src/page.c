/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaraval <tmaraval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 15:07:40 by tmaraval          #+#    #+#             */
/*   Updated: 2019/10/10 12:25:03 by tmaraval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	page_free_fill_chunk_to_free(t_zone **current_zone)
{
	int		p;
	t_chunk *chunk_check;
	t_chunk *chunk_keep;
	int		state;
	int		i;

	p = 0;
	chunk_check = (*current_zone)->chunks;
	chunk_keep = chunk_check;
	while (p < (*current_zone)->pages_nbr)
	{
		state = 0;
		i = 0;
#ifdef DEBUG_PAGE
		ft_putstr("page = ");
		ft_putnbr(p);
		ft_putendl("");
#endif
		if (p > 0 && chunk_check && chunk_check->prev && chunk_check != (void *)(*current_zone) + getpagesize() * (p))
			chunk_check = chunk_check->prev;
		chunk_keep = chunk_check;
		while (chunk_check && (void *)chunk_check < (void *)(*current_zone) + getpagesize() * (p + 1))
		{
#ifdef DEBUG_PAGE
			ft_putendl("chunk_check : ");
			print_chunk(chunk_check);
#endif
			if (chunk_check->status == TO_REMOVE)
				state += 1;
			else
				state += chunk_check->status;
			i++;
			chunk_check = chunk_check->next;
		}
#ifdef DEBUG_PAGE
		ft_putstr("chunk_keep = ");
		ft_putptr(chunk_keep);
		ft_putendl("");
#endif
		if (state == i && state != 0)
		{
			if (p > 0)
			{
				while (chunk_keep && (void *)chunk_keep < (void *)(*current_zone) + getpagesize() * (p + 1))
				{
#ifdef DEBUG_PAGE
					ft_putendl("added chunk : ");
					print_chunk(chunk_keep);
#endif
					chunk_keep->status = TO_REMOVE;	
					chunk_keep = chunk_keep->next;
				}
				(*current_zone)->state[p] = TO_FREE;
			}
		}
		p++;
	}

}

void	page_free(t_zone **current_zone)
{
	t_chunk *chunk_to_free;
	t_chunk	*cpy;
	int i;

	i = 0;
	chunk_to_free = NULL;
	page_free_fill_chunk_to_free(current_zone);
#ifdef DEBUG_PAGE
	print_chunks((*current_zone)->chunks, "chunk to free : ");
	ft_putstr("page to free : \n");
	while (i < (*current_zone)->pages_nbr)
	{
		ft_putnbr(i);
		ft_putstr("  =  ");
		ft_putnbr((*current_zone)->state[i]);
		ft_putendl("");
		i++;
	}
#endif
	chunk_to_free = (*current_zone)->chunks;
	while (chunk_to_free)
	{
		cpy =  chunk_to_free;
		if (chunk_to_free->status == TO_REMOVE)
			delete_chunk(&((*current_zone)->chunks), chunk_to_free);
		chunk_to_free = cpy->next;
	}
#ifdef DEBUG_PAGE
	print_chunks((*current_zone)->chunks, "after delete");
#endif
	i = 0;
	while (i < (*current_zone)->pages_nbr)
	{
		if ((*current_zone)->state[i] == TO_FREE)
		{
			munmap(*current_zone + (getpagesize() * i), getpagesize());
			(*current_zone)->state[i] = FREE;
		}
		i++;
	}
}
