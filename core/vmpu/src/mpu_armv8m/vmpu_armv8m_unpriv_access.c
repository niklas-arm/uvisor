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
#include "context.h"
#include "debug.h"
#include "vmpu.h"
#include "vmpu_mpu.h"
#include "vmpu_unpriv_access.h"
#include <stdbool.h>
#include <arm_cmse.h>

extern void debug_sau_config(void);

uint32_t vmpu_unpriv_access(uint32_t addr, uint32_t data, uint32_t size, uint32_t op)
{
    if ((size > 4) || !vmpu_buffer_access_is_ok(g_active_box, (void *) addr, size)) {
        DPRINTF("0x%08x/%d NOT allowed.\n!", addr, size);
        debug_sau_config();
        HALT_ERROR(PERMISSION_DENIED, "Access to restricted resource denied");
        return 0;
    }
    addr = UVISOR_GET_S_ALIAS(addr);
    switch(size | op) {
        case (UVISOR_UNPRIV_ACCESS_OP_READ | 1):
            return *((uint8_t *) addr);
        case (UVISOR_UNPRIV_ACCESS_OP_READ | 2):
            return *((uint16_t *) addr);
        case (UVISOR_UNPRIV_ACCESS_OP_READ | 4):
            return *((uint32_t *) addr);
        case (UVISOR_UNPRIV_ACCESS_OP_WRITE | 1):
            *((uint8_t *) addr) = (uint8_t) data;
            return 0;
        case (UVISOR_UNPRIV_ACCESS_OP_WRITE | 2):
            *((uint16_t *) addr) = (uint16_t) data;
            return 0;
        case (UVISOR_UNPRIV_ACCESS_OP_WRITE | 4):
            *((uint32_t *) addr) = data;
            return 0;
        default:
            break;
    }
    return 0;
}
