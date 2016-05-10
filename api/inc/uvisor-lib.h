/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_UVISOR_LIB_H__
#define __UVISOR_API_UVISOR_LIB_H__

/* This file includes all the uVisor library header files at once.
 * If uVisor is used on a host OS that includes unsupported targets, then
 * unsupported.h is included, which defines a fallback version of those APIs,
 * with no security feature. */

#if defined(UVISOR_PRESENT) && UVISOR_PRESENT == 1

/* Library header files */
#include "api/inc/benchmark.h"
#include "api/inc/box_config.h"
#include "api/inc/debug.h"
#include "api/inc/disabled.h"
#include "api/inc/error.h"
#include "api/inc/interrupts.h"
#include "api/inc/privcall.h"
#include "api/inc/register_gateway.h"
#include "api/inc/secure_access.h"
#include "api/inc/secure_gateway.h"

#else /* defined(UVISOR_PRESENT) && UVISOR_PRESENT == 1 */

#include "api/inc/unsupported.h"

#endif /* defined(UVISOR_PRESENT) && UVISOR_PRESENT == 1 */

#include "api/inc/page_allocator.h"

/* Include all exported header files used by uVisor internally.
 * These are included independently on whether uVisor is supported or not by the
 * target platform. */
#include "api/inc/debug_exports.h"
#include "api/inc/halt_exports.h"
#include "api/inc/svc_exports.h"
#include "api/inc/svc_gw_exports.h"
#include "api/inc/unvic_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/vmpu_exports.h"
#include "api/inc/page_allocator_exports.h"

#endif /* __UVISOR_API_UVISOR_LIB_H__ */
