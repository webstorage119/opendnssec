/*
 * Copyright (c) 2009 NLNet Labs. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * The hard workers.
 *
 */

#ifndef DAEMON_WORKER_H
#define DAEMON_WORKER_H

#include "config.h"
#include <time.h>

enum worker_enum {
    WORKER_NONE = 0,
    WORKER_WORKER = 1,
    WORKER_DRUDGER
};
typedef enum worker_enum worker_id;

typedef struct worker_struct worker_type;

#include "scheduler/task.h"
#include "status.h"
#include "locks.h"

struct worker_struct {
    char* name;
    ods_thread_type thread_id;
    engine_type* engine;
    time_t clock_in;
    int tasksOutstanding;
    int tasksFailed;
    pthread_cond_t tasksBlocker;
    unsigned need_to_exit : 1;
};

struct worker_context {
    worker_type* worker;
    task_type* task;
};

/**
 * Create worker.
 * \param[in] allocator memory allocator
 * \param[in] num thread number
 * \param[in] type type of worker
 * \return worker_type* created worker
 *
 */
worker_type* worker_create(char* name);

/**
 * Start working.
 * \param[in] worker worker to start working
 *
 */
void worker_drudge(worker_type* worker);
void worker_work(worker_type* worker);

/**
 * Notify all workers.
 * \param[in] lock lock to use
 * \param[in] condition condition that has been met
 *
 */
void worker_notify_all(pthread_mutex_t* lock, pthread_cond_t* condition);

/**
 * Clean up worker.
 * \param[in] worker worker to clean up
 *
 */
void worker_cleanup(worker_type* worker);

time_t worker_perform_task(const char* zonename, void* zonearg, void* contextarg);

#endif /* DAEMON_WORKER_H */
