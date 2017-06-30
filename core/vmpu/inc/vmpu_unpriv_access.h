/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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
#ifndef __VMPU_UNPRIV_ACCESS_H__
#define __VMPU_UNPRIV_ACCESS_H__

#include "api/inc/uvisor_exports.h"
#include "halt.h"
#include <stdint.h>

#if defined(ARCH_MPU_ARMv8M)

#define UVISOR_UNPRIV_ACCESS_OP_READ  0b00000
#define UVISOR_UNPRIV_ACCESS_OP_WRITE 0b10000

extern uint32_t vmpu_unpriv_access(uint32_t addr, uint32_t data, uint32_t size, uint32_t op);

#endif

typedef enum
{
    XPRIV_MEMCMP_ADDR1_UNPRIVILEGED = 0,
    XPRIV_MEMCMP_ADDR1_PRIVILEGED = 0b10,
    XPRIV_MEMCMP_ADDR2_UNPRIVILEGED = 0,
    XPRIV_MEMCMP_ADDR2_PRIVILEGED = 0b01,
} XPrivMemCmpType;

extern int vmpu_xpriv_memcmp(uint32_t addr1, uint32_t addr2, size_t length, XPrivMemCmpType type);

/** Write an 8-bit value to an address as if in unprivileged mode.
 *
 * This function can be used even if the MPU is disabled, but the architectural
 * restrictions on privileged-only registers still apply.
 *
 * @warning: Upon failure this function triggers a fault (MemManage if enabled,
 *
 * @param addr[in]  Address to write to.
 * @param data[in]  8-bit data to write.
 */
static UVISOR_FORCEINLINE void vmpu_unpriv_uint8_write(uint32_t addr, uint8_t data)
{
#if defined(ARCH_MPU_ARMv8M)
    vmpu_unpriv_access(addr, data, sizeof(uint8_t), UVISOR_UNPRIV_ACCESS_OP_WRITE);
#else
    asm volatile (
        "strbt %[data], [%[addr]]\n"
        :: [addr] "r" (addr), [data] "r" (data)
    );
#endif
}

/** Write a 16-bit value to an address as if in unprivileged mode.
 *
 * This function can be used even if the MPU is disabled, but the architectural
 * restrictions on privileged-only registers still apply.
 *
 * @warning: Upon failure this function triggers a fault (MemManage if enabled,
 * otherwise HardFault).
 *
 * @param addr[in]  Address to write to.
 * @param data[in]  16-bit data to write.
 */
static UVISOR_FORCEINLINE void vmpu_unpriv_uint16_write(uint32_t addr, uint16_t data)
{
#if defined(ARCH_MPU_ARMv8M)
    vmpu_unpriv_access(addr, data, sizeof(uint16_t), UVISOR_UNPRIV_ACCESS_OP_WRITE);
#else
    asm volatile (
        "strht %[data], [%[addr]]\n"
        :: [addr] "r" (addr), [data] "r" (data)
    );
#endif
}

/** Write a 32-bit value to an address as if in unprivileged mode.
 *
 * This function can be used even if the MPU is disabled, but the architectural
 * restrictions on privileged-only registers still apply.
 *
 * @warning: Upon failure this function triggers a fault (MemManage if enabled,
 * otherwise HardFault).
 *
 * @param addr[in]  Address to write to.
 * @param data[in]  32-bit data to write.
 */
static UVISOR_FORCEINLINE void vmpu_unpriv_uint32_write(uint32_t addr, uint32_t data)
{
#if defined(ARCH_MPU_ARMv8M)
    vmpu_unpriv_access(addr, data, sizeof(uint32_t), UVISOR_UNPRIV_ACCESS_OP_WRITE);
#else
    asm volatile (
        "strt %[data], [%[addr]]\n"
        :: [addr] "r" (addr), [data] "r" (data)
    );
#endif
}

/** Read an 8-bit value from an address as if in unprivileged mode.
 *
 * This function can be used even if the MPU is disabled, but the architectural
 * restrictions on privileged-only registers still apply.
 *
 * @warning: Upon failure this function triggers a fault (MemManage if enabled,
 * otherwise HardFault).
 *
 * @param addr[in]  Address to read from.
 * @returns the 8-bit value read from the address.
 */
static UVISOR_FORCEINLINE uint8_t vmpu_unpriv_uint8_read(uint32_t addr)
{
#if defined(ARCH_MPU_ARMv8M)
    return (uint8_t) vmpu_unpriv_access(addr, 0, sizeof(uint8_t), UVISOR_UNPRIV_ACCESS_OP_READ);
#else
    uint8_t res;
    asm volatile (
        "ldrbt %[res], [%[addr]]\n"
        : [res] "=r" (res)
        : [addr] "r" (addr)
    );
    return res;
#endif
}

/** Read a 16-bit value from an address as if in unprivileged mode.
 *
 * This function can be used even if the MPU is disabled, but the architectural
 * restrictions on privileged-only registers still apply.
 *
 * @warning: Upon failure this function triggers a fault (MemManage if enabled,
 * otherwise HardFault).
 *
 * @param addr[in]  Address to read from.
 * @returns the 16-bit value read from the address.
 */
static UVISOR_FORCEINLINE uint16_t vmpu_unpriv_uint16_read(uint32_t addr)
{
#if defined(ARCH_MPU_ARMv8M)
    return (uint16_t) vmpu_unpriv_access(addr, 0, sizeof(uint16_t), UVISOR_UNPRIV_ACCESS_OP_READ);
#else
    uint16_t res;
    asm volatile (
        "ldrht %[res], [%[addr]]\n"
        : [res] "=r" (res)
        : [addr] "r" (addr)
    );
    return res;
#endif
}

/** Read a 32-bit value from an address as if in unprivileged mode.
 *
 * This function can be used even if the MPU is disabled, but the architectural
 * restrictions on privileged-only registers still apply.
 *
 * @warning: Upon failure this function triggers a fault (MemManage if enabled,
 * otherwise HardFault).
 *
 * @param addr[in]  Address to read from.
 * @returns the 32-bit value read from the address.
 */
static UVISOR_FORCEINLINE uint32_t vmpu_unpriv_uint32_read(uint32_t addr)
{
#if defined(ARCH_MPU_ARMv8M)
    return vmpu_unpriv_access(addr, 0, sizeof(uint32_t), UVISOR_UNPRIV_ACCESS_OP_READ);
#else
    uint32_t res;
    asm volatile (
        "ldrt %[res], [%[addr]]\n"
        : [res] "=r" (res)
        : [addr] "r" (addr)
    );
    return res;
#endif
}

static UVISOR_FORCEINLINE int vmpu_unpriv_unpriv_memcmp(uint32_t addr1, uint32_t addr2, size_t length)
{
    return vmpu_xpriv_memcmp(addr1, addr2, length, XPRIV_MEMCMP_ADDR1_UNPRIVILEGED | XPRIV_MEMCMP_ADDR2_UNPRIVILEGED);
}

static UVISOR_FORCEINLINE int vmpu_unpriv_priv_memcmp(uint32_t addr1, uint32_t addr2, size_t length)
{
    return vmpu_xpriv_memcmp(addr1, addr2, length, XPRIV_MEMCMP_ADDR1_UNPRIVILEGED | XPRIV_MEMCMP_ADDR2_PRIVILEGED);
}

static UVISOR_FORCEINLINE int vmpu_priv_unpriv_memcmp(uint32_t addr1, uint32_t addr2, size_t length)
{
    return vmpu_xpriv_memcmp(addr1, addr2, length, XPRIV_MEMCMP_ADDR1_PRIVILEGED | XPRIV_MEMCMP_ADDR2_UNPRIVILEGED);
}

static UVISOR_FORCEINLINE int vmpu_priv_priv_memcmp(uint32_t addr1, uint32_t addr2, size_t length)
{
    return vmpu_xpriv_memcmp(addr1, addr2, length, XPRIV_MEMCMP_ADDR1_PRIVILEGED | XPRIV_MEMCMP_ADDR2_PRIVILEGED);
}

#endif /* __VMPU_UNPRIV_ACCESS_H__ */
