#ifndef _XT_BPF_H
#define _XT_BPF_H

#include <linux/filter.h>
<<<<<<< HEAD
#include <linux/limits.h>
#include <linux/types.h>

#define XT_BPF_MAX_NUM_INSTR	64
#define XT_BPF_PATH_MAX		(XT_BPF_MAX_NUM_INSTR * sizeof(struct sock_filter))
=======
#include <linux/types.h>

#define XT_BPF_MAX_NUM_INSTR	64
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

struct bpf_prog;

struct xt_bpf_info {
	__u16 bpf_program_num_elem;
	struct sock_filter bpf_program[XT_BPF_MAX_NUM_INSTR];

	/* only used in the kernel */
	struct bpf_prog *filter __attribute__((aligned(8)));
};

<<<<<<< HEAD
enum xt_bpf_modes {
	XT_BPF_MODE_BYTECODE,
	XT_BPF_MODE_FD_PINNED,
	XT_BPF_MODE_FD_ELF,
};
#define XT_BPF_MODE_PATH_PINNED XT_BPF_MODE_FD_PINNED

struct xt_bpf_info_v1 {
	__u16 mode;
	__u16 bpf_program_num_elem;
	__s32 fd;
	union {
		struct sock_filter bpf_program[XT_BPF_MAX_NUM_INSTR];
		char path[XT_BPF_PATH_MAX];
	};

	/* only used in the kernel */
	struct bpf_prog *filter __attribute__((aligned(8)));
};

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
#endif /*_XT_BPF_H */
