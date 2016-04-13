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
#ifndef __UVISOR_API_PAGE_ALLOCATOR_EXPORTS_H__
#define __UVISOR_API_PAGE_ALLOCATOR_EXPORTS_H__

#include <stdint.h>

/* Must be a power of 2 for MPU alignment in ARMv7-M */
#ifndef UVISOR_PAGE_SIZE
#   define UVISOR_PAGE_SIZE ((size_t)16 * 1024)
#endif

/* Returns the rounded up number of pages required to hold `size` */
#define UVISOR_PAGES_FOR_SIZE(size) \
    ((size + UVISOR_PAGE_SIZE - 1) / UVISOR_PAGE_SIZE)

/* creates a page table with `count` many entries */
#define UVISOR_PAGE_TABLE(count) \
    struct { \
        size_t page_size; \
        size_t page_count; \
        void* page_origins[count]; \
    }

/* creates a page table with enough pages to hold `size` */
#define UVISOR_PAGE_TABLE_FOR_SIZE(size) \
    UVISOR_PAGE_TABLE(UVISOR_PAGES_FOR_SIZE(size))


typedef struct {
    size_t page_size;       /* the page size in bytes */
    size_t page_count;      /* the number of pages in the page table */
    void* page_origins[1];  /* table of pointer to the origins of a page */
} UvisorPageTable;

#endif /* __UVISOR_API_PAGE_ALLOCATOR_EXPORTS_H__ */
