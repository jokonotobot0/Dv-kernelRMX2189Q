/*
 * Copyright 2006, Red Hat, Inc., Dave Jones
 * Released under the General Public License (GPL).
 *
<<<<<<< HEAD
 * This file contains the linked list validation for DEBUG_LIST.
=======
 * This file contains the linked list implementations for
 * DEBUG_LIST.
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
 */

#include <linux/export.h>
#include <linux/list.h>
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/rculist.h>

/*
<<<<<<< HEAD
 * Check that the data structures for the list manipulations are reasonably
 * valid. Failures here indicate memory corruption (and possibly an exploit
 * attempt).
 */

bool __list_add_valid(struct list_head *new, struct list_head *prev,
		      struct list_head *next)
{
	if (CHECK_DATA_CORRUPTION(next->prev != prev,
			"list_add corruption. next->prev should be prev (%p), but was %p. (next=%p).\n",
			prev, next->prev, next) ||
	    CHECK_DATA_CORRUPTION(prev->next != next,
			"list_add corruption. prev->next should be next (%p), but was %p. (prev=%p).\n",
			next, prev->next, prev) ||
	    CHECK_DATA_CORRUPTION(new == prev || new == next,
			"list_add double add: new=%p, prev=%p, next=%p.\n",
			new, prev, next))
		return false;

	return true;
}
EXPORT_SYMBOL(__list_add_valid);

bool __list_del_entry_valid(struct list_head *entry)
=======
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */

void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	WARN(next->prev != prev,
		"list_add corruption. next->prev should be "
		"prev (%p), but was %p. (next=%p).\n",
		prev, next->prev, next);
	WARN(prev->next != next,
		"list_add corruption. prev->next should be "
		"next (%p), but was %p. (prev=%p).\n",
		next, prev->next, prev);
	WARN(new == prev || new == next,
	     "list_add double add: new=%p, prev=%p, next=%p.\n",
	     new, prev, next);
	next->prev = new;
	new->next = next;
	new->prev = prev;
	WRITE_ONCE(prev->next, new);
}
EXPORT_SYMBOL(__list_add);

void __list_del_entry(struct list_head *entry)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	struct list_head *prev, *next;

	prev = entry->prev;
	next = entry->next;

<<<<<<< HEAD
	if (CHECK_DATA_CORRUPTION(next == LIST_POISON1,
			"list_del corruption, %p->next is LIST_POISON1 (%p)\n",
			entry, LIST_POISON1) ||
	    CHECK_DATA_CORRUPTION(prev == LIST_POISON2,
			"list_del corruption, %p->prev is LIST_POISON2 (%p)\n",
			entry, LIST_POISON2) ||
	    CHECK_DATA_CORRUPTION(prev->next != entry,
			"list_del corruption. prev->next should be %p, but was %p\n",
			entry, prev->next) ||
	    CHECK_DATA_CORRUPTION(next->prev != entry,
			"list_del corruption. next->prev should be %p, but was %p\n",
			entry, next->prev))
		return false;

	return true;

}
EXPORT_SYMBOL(__list_del_entry_valid);
=======
	if (WARN(next == LIST_POISON1,
		"list_del corruption, %p->next is LIST_POISON1 (%p)\n",
		entry, LIST_POISON1) ||
	    WARN(prev == LIST_POISON2,
		"list_del corruption, %p->prev is LIST_POISON2 (%p)\n",
		entry, LIST_POISON2) ||
	    WARN(prev->next != entry,
		"list_del corruption. prev->next should be %p, "
		"but was %p\n", entry, prev->next) ||
	    WARN(next->prev != entry,
		"list_del corruption. next->prev should be %p, "
		"but was %p\n", entry, next->prev))
		return;

	__list_del(prev, next);
}
EXPORT_SYMBOL(__list_del_entry);

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
void list_del(struct list_head *entry)
{
	__list_del_entry(entry);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}
EXPORT_SYMBOL(list_del);

/*
 * RCU variants.
 */
void __list_add_rcu(struct list_head *new,
		    struct list_head *prev, struct list_head *next)
{
	WARN(next->prev != prev,
		"list_add_rcu corruption. next->prev should be prev (%p), but was %p. (next=%p).\n",
		prev, next->prev, next);
	WARN(prev->next != next,
		"list_add_rcu corruption. prev->next should be next (%p), but was %p. (prev=%p).\n",
		next, prev->next, prev);
	new->next = next;
	new->prev = prev;
	rcu_assign_pointer(list_next_rcu(prev), new);
	next->prev = new;
}
EXPORT_SYMBOL(__list_add_rcu);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
