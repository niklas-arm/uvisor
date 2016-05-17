/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <uvisor.h>
#include "vmpu.h"
#include "api/inc/privcall_exports.h"

typedef struct {
    void *allocator;
    uint8_t process_id;
    int thread_id;
} UvisorThreadContext;

#define UVISOR_MAX_THREADS 20
// #define UVISOR_MAX_THREADS OS_TASKCNT
UvisorThreadContext thread[UVISOR_MAX_THREADS] = {0};


void privcall_box_switch(uint8_t dst_box)
{
    /* We trust the vmpu_switch function to check the validity of the source and
     * destination IDs. */
    vmpu_switch(g_active_box, dst_box);
    g_active_box = dst_box;
}

static int thread_ctx_valid(UvisorThreadContext *context)
{
    /* check if context pointer points into the array */
    if ((void*)context < (void*)&thread ||
        ((void*)&thread + sizeof(thread)) <= (void*)context) {
        return 0;
    }
    /* check if the context is aligned exactly to a context */
    return ( (((void*)context - (void*)thread) % sizeof(UvisorThreadContext)) == 0 );
}

int privcall_get_process_id(void *context) {

    if (context == NULL) return -1;

    if (thread_ctx_valid(context)) {
        return ((UvisorThreadContext*)context)->process_id;
    }
    else {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "thread context (%08x) is invalid!\n",
            context);
    }
    return -1;
}

void *privcall_thread_create(int id, void *context)
{
    const UvisorBoxIndex *const index =
            (UvisorBoxIndex *const) *(__uvisor_config.uvisor_box_context);
    /* search for a free slot in the tasks meta data */
    int ii = 0;
    for (; ii < UVISOR_MAX_THREADS; ii++) {
        if (thread[ii].allocator == NULL)
            break;
    }
    if (ii < UVISOR_MAX_THREADS) {
        thread[ii].thread_id = id;
        /* remember the process id as well */
        thread[ii].process_id = g_active_box;
        /* fall back to the process heap if ctx is NULL */
        thread[ii].allocator = context ? context : index->process_heap;
        return &thread[ii];
    }
    return context;
}

void privcall_thread_destroy(void *context)
{
    if (context == NULL) return;

    /* only if TID is valid and destruction status is zero */
    if (thread_ctx_valid(context) &&
        (((UvisorThreadContext*)context)->process_id == g_active_box)) {
        /* release this slot */
        ((UvisorThreadContext*)context)->allocator = NULL;
    }
    else {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "thread context (%08x) is invalid!\n",
            context);
    }
}

void privcall_thread_switch(void *context)
{
    if (context == NULL) return;

    UvisorBoxIndex *const index =
            (UvisorBoxIndex *const) *(__uvisor_config.uvisor_box_context);
    /* only if TID is valid and the slot is used */
    if (!thread_ctx_valid(context)) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "thread context (%08x) is invalid!\n",
            context);
        return;
    }
    if (((UvisorThreadContext*)context)->process_id != g_active_box) {
        privcall_box_switch(((UvisorThreadContext*)context)->process_id);
    }
    if (((UvisorThreadContext*)context)->allocator) {
        /* if the active_heap is NULL, then the process heap needs to be
         * initialized yet. The initializer sets the active heap itself. */
        if (index->active_heap) {
            index->active_heap = ((UvisorThreadContext*)context)->allocator;
        }
    }
}

const struct uvisor_privcall_table uvisor_privcall = {
    .version = UVISOR_PRIVCALL_VERSION,
    .process_switch = privcall_box_switch,
    .get_process_id = privcall_get_process_id,
    .active_process = &g_active_box,
    .thread_create = privcall_thread_create,
    .thread_destroy = privcall_thread_destroy,
    .thread_switch = privcall_thread_switch,
};
