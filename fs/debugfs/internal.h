<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
/*
 *  internal.h - declarations internal to debugfs
 *
 *  Copyright (C) 2016 Nicolai Stange <nicstange@gmail.com>
<<<<<<< HEAD
=======
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
 *
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
 */

#ifndef _DEBUGFS_INTERNAL_H_
#define _DEBUGFS_INTERNAL_H_

struct file_operations;

/* declared over in file.c */
extern const struct file_operations debugfs_noop_file_operations;
extern const struct file_operations debugfs_open_proxy_file_operations;
extern const struct file_operations debugfs_full_proxy_file_operations;

<<<<<<< HEAD
struct debugfs_fsdata {
	const struct file_operations *real_fops;
	refcount_t active_users;
	struct completion active_users_drained;
};

/*
 * A dentry's ->d_fsdata either points to the real fops or to a
 * dynamically allocated debugfs_fsdata instance.
 * In order to distinguish between these two cases, a real fops
 * pointer gets its lowest bit set.
 */
#define DEBUGFS_FSDATA_IS_REAL_FOPS_BIT BIT(0)

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
#endif /* _DEBUGFS_INTERNAL_H_ */
