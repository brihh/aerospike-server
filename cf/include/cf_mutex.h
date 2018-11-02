/*
 * cf_mutex.h
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

#pragma once


//==========================================================
// Includes.
//

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>


//==========================================================
// Typedefs & constants.
//

#define cf_mutex pthread_mutex_t
#define cf_condition pthread_cond_t

#define CF_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define cf_mutex_init(__m) pthread_mutex_init(__m,NULL)
#define cf_mutex_destroy(__m) pthread_mutex_destroy(__m)


//==========================================================
// Public API.
//

void cf_mutex_lock(cf_mutex *m);
void cf_mutex_unlock(cf_mutex *m);
bool cf_mutex_trylock(cf_mutex *m);

void cf_mutex_lock_spin(cf_mutex *m);
void cf_mutex_unlock_spin(cf_mutex *m);

void cf_condition_wait(cf_condition *c, cf_mutex *m);
void cf_condition_signal(cf_condition *c);
