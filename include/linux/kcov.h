#ifndef _LINUX_KCOV_H
#define _LINUX_KCOV_H

#include <uapi/linux/kcov.h>

struct task_struct;

#ifdef CONFIG_KCOV

<<<<<<< HEAD
enum kcov_mode {
	/* Coverage collection is not enabled yet. */
	KCOV_MODE_DISABLED = 0,
	/* KCOV was initialized, but tracing mode hasn't been chosen yet. */
	KCOV_MODE_INIT = 1,
=======
void kcov_task_init(struct task_struct *t);
void kcov_task_exit(struct task_struct *t);

enum kcov_mode {
	/* Coverage collection is not enabled yet. */
	KCOV_MODE_DISABLED = 0,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	/*
	 * Tracing coverage collection mode.
	 * Covered PCs are collected in a per-task buffer.
	 */
<<<<<<< HEAD
	KCOV_MODE_TRACE_PC = 2,
	/* Collecting comparison operands mode. */
	KCOV_MODE_TRACE_CMP = 3,
};

void kcov_task_init(struct task_struct *t);
void kcov_task_exit(struct task_struct *t);

=======
	KCOV_MODE_TRACE = 1,
};

>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
#else

static inline void kcov_task_init(struct task_struct *t) {}
static inline void kcov_task_exit(struct task_struct *t) {}

#endif /* CONFIG_KCOV */
#endif /* _LINUX_KCOV_H */
