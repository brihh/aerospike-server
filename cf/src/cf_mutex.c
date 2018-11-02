/*
 * cf_mutex.c
 *
 * Copyright (C) 2017 Aerospike, Inc.
 *
 * Portions may be licensed to Aerospike, Inc. under one or more contributor
 * license agreements.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */


//==========================================================
// Includes.
//

#include <cf_mutex.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <linux/futex.h>
#include <sys/syscall.h>

#include "fault.h"


//==========================================================
// Typedefs & constants.
//

#define FUTEX_SPIN_MAX 100


//==========================================================
// Inlines & macros.
//

inline static void
sys_futex(void *uaddr, int op, int val)
{
	syscall(SYS_futex, uaddr, op, val, NULL, NULL, 0);
}

#define xchg(__ptr, __val) __sync_lock_test_and_set(__ptr, __val)
#define cmpxchg(__ptr, __cmp, __set) __sync_val_compare_and_swap(__ptr, __cmp, __set)

#if defined(__powerpc64__)
#include <sys/platform/ppc.h>
#define cpu_relax() __ppc_get_timebase()
#else
#define cpu_relax() asm volatile("pause\n": : :"memory")
#endif

#define unlikely(__expr) __builtin_expect(!! (__expr), 0)
#define likely(__expr) __builtin_expect(!! (__expr), 1)


//==========================================================
// Public API - cf_mutex.
//

void
cf_mutex_lock(cf_mutex *m)
{
	int rc = pthread_mutex_lock(m);
	if (unlikely(rc != 0)) {
		cf_crash(CF_MISC, "cf_mutex_lock() failed");
	}
}

void
cf_mutex_unlock(cf_mutex *m)
{
	int rc = pthread_mutex_unlock(m);
	if (unlikely(rc != 0)) {
		cf_crash(CF_MISC, "cf_mutex_unlock() failed");
	}
}

// Return true if lock success.
bool
cf_mutex_trylock(cf_mutex *m)
{
	int rc = pthread_mutex_lock(m);
	return rc;
}

void
cf_mutex_lock_spin(cf_mutex *m)
{
	int rc = pthread_mutex_lock(m);
	if (unlikely(rc != 0)) {
		cf_crash(CF_MISC, "cf_mutex_lock_spin() failed");
	}
}

void
cf_mutex_unlock_spin(cf_mutex *m)
{
	int rc = pthread_mutex_unlock(m);
	if (unlikely(rc != 0)) {
		cf_crash(CF_MISC, "cf_mutex_unlock_spin() failed");
	}
}


//==========================================================
// Public API - cf_condition.
//

void
cf_condition_wait(cf_condition *c, cf_mutex *m)
{
	int rc = pthread_cond_wait(c, m);
	if (unlikely(rc != 0)) {
		cf_crash(CF_MISC, "cf_condition_wait() failed");
	}
}

void
cf_condition_signal(cf_condition *c)
{
	int rc = pthread_cond_signal(c);
	if (unlikely(rc != 0)) {
		cf_crash(CF_MISC, "cf_condition_signal() failed");
	}
}

