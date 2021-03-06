/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_zone.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaraval <tmaraval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 14:09:07 by tmaraval          #+#    #+#             */
/*   Updated: 2019/10/10 14:12:52 by tmaraval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#include "tests.h"

void	test_shrink_zone_shrink(void **state)
{
	t_zone *zone;

	zone = NULL;
	add_zone(&zone, 16);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->pages_nbr, 16);
	zone->used = 4096+100;
	shrink_zone(&zone);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->used, 4096+100);
	assert_int_equal(zone->size, 4096*2);
	assert_int_equal(zone->state[2], FREE);
	assert_int_equal(zone->state[3], FREE);
	assert_int_equal(zone->pages_nbr, 2);	
}

void	test_shrink_zone_noShrink(void **state)
{
	t_zone *zone;

	zone = NULL;
	add_zone(&zone, 16);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->pages_nbr, 16);
	zone->used = 65436;
	shrink_zone(&zone);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->used, 65436);
	assert_int_equal(zone->pages_nbr, 16);	
}

void	test_delete_zone(void **state)
{
	t_zone *zone;
	t_zone *zone1;
	t_zone *zone2;
	t_zone *zone3;

	zone = NULL;
	add_zone(&zone, 16);
	add_zone(&zone, 16);
	add_zone(&zone, 16);
	zone1 = zone;
	zone2 = zone->next;
	zone3 = zone->next->next;
	assert_int_equal(cnt_zone(zone), 3);
	delete_zone(&zone, zone->next, getpagesize());
	assert_ptr_equal(zone->prev, NULL);
	assert_ptr_equal(zone->next, zone3);
	assert_ptr_equal(zone->next->prev, zone);
	assert_int_equal(cnt_zone(zone), 2);
	delete_zone(&zone, zone, getpagesize());
	assert_ptr_equal(zone, zone3);
	assert_ptr_equal(zone->next, NULL);
	assert_ptr_equal(zone->prev, NULL);
	assert_int_equal(cnt_zone(zone), 1);
	delete_zone(&zone, zone, getpagesize());
	assert_int_equal(cnt_zone(zone), 0);
}

void	test_should_delete_zone(void **state)
{
	t_zone *zone;
	int i;

	i = 0;
	zone = NULL;
	add_zone(&zone, 16);
	add_zone(&zone, 16);
	add_zone(&zone, 16);
	while (i < 16)
	{
		zone->state[i] = FREE;
		i++;
	}
	zone->next->next->state[2] = FREE;
	zone->next->next->state[5] = FREE;
	zone->next->next->state[8] = FREE;
	assert_int_equal(should_delete_zone(zone), 1);
	assert_int_equal(should_delete_zone(zone->next), 0);
	assert_int_equal(should_delete_zone(zone->next->next), 0);
}

void test_search_zone_needAdd(void **state)
{
	t_zone *zone;
	t_zone *ret;

	ret = NULL;
	zone = NULL;
	add_zone(&zone, 16);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->size, 4096*16 - sizeof(t_zone));
	assert_int_equal(zone->used, 0);
	zone->used = getpagesize() * 15;
	ret = search_zone(&zone, getpagesize());
	assert_ptr_not_equal(zone, ret);
	assert_int_equal(zone->next->used, 0);
}


void test_search_zone_same(void **state)
{
	t_zone *zone;
	
	zone = NULL;
	add_zone(&zone, 16);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->size, 4096*16 - sizeof(t_zone));
	assert_int_equal(zone->used, 0);
	assert_ptr_equal(search_zone(&zone, 32), zone);
}

void	test_add_zone_large(void **state)
{
	t_zone *zone;

	zone = NULL;
	add_zone_large(&zone, 4096);
	add_zone_large(&zone, 8096);
	add_zone_large(&zone, 16096);
	assert_int_equal(cnt_zone(zone), 3);
	assert_int_equal(zone->size, 4096);
	assert_int_equal(zone->next->size, 8096);
	assert_int_equal(zone->next->next->size, 16096);
}

void test_add_zone_after(void **state)
{
	t_zone *zone;
	
	zone = NULL;
	add_zone(&zone, 16);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->size, 4096*16 - sizeof(t_zone));
	assert_int_equal(zone->used, 0);
	assert_int_equal(zone->pages_nbr, 16);
	assert_ptr_equal(zone->chunks, NULL);
	assert_int_equal(add_zone(&zone,16), 0);
	assert_ptr_equal(zone, zone->next->prev);
	assert_int_equal(zone->next->size, 4096*16 - sizeof(t_zone));
	assert_int_equal(zone->next->used, 0);
	assert_int_equal(zone->next->pages_nbr, 16);
	assert_ptr_equal(zone->next->chunks, NULL);
}

void test_add_zone_null(void **state)
{
	t_zone *zone;

	zone = NULL;
	assert_int_equal(add_zone(&zone,16), 0);
	assert_ptr_not_equal(zone, NULL);
	assert_int_equal(zone->size, 4096*16 - sizeof(t_zone));
	assert_int_equal(zone->used, 0);
	assert_int_equal(zone->pages_nbr, 16);
	assert_ptr_equal(zone->chunks, NULL);
}
