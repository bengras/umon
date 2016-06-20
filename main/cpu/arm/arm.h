/**************************************************************************
 *
 * Copyright (c) 2013 Alcatel-Lucent
 *
 * Alcatel Lucent licenses this file to You under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  A copy of the License is contained the
 * file LICENSE at the top level of this repository.
 * You may also obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************
 *
 * arm.h
 *
 * PSR Bits
 *
 * Original author:     Ed Sutter (ed.sutter@alcatel-lucent.com)
 *
 */
#define PSR_THUMB_STATE         0x00000020
#define PSR_IMASK_IRQ           0x00000080
#define PSR_IMASK_FRQ           0x00000040
#define PSR_CONDITION_NEGATIVE  0x80000000
#define PSR_CONDITION_ZERO      0x40000000
#define PSR_CONDITION_CARRY     0x20000000
#define PSR_CONDITION_OVERFLOW  0x10000000
#define PSR_MODE_MASK           0x0000001f

/* Mode bits within PSR:
 */
#define ABORT_MODE          0x00000017
#define FASTINTRQST_MODE    0x00000011
#define INTRQST_MODE        0x00000012
#define SUPERVISOR_MODE     0x00000013
#define SYSTEM_MODE         0x0000001f
#define UNDEFINED_MODE      0x0000001b
#define USER_MODE           0x00000010

/* Exception types:
 */
#define EXCTYPE_UNDEF       1
#define EXCTYPE_ABORTP      2
#define EXCTYPE_ABORTD      3
#define EXCTYPE_IRQ         4
#define EXCTYPE_FIRQ        5
#define EXCTYPE_SWI         6
#define EXCTYPE_NOTASSGN    7

/* Link register adjustments for each exception:
 * These adjustments are used by the exception handler to establish the
 * address at which the exception occurred.
 */
#define LRADJ_UNDEF         4
#define LRADJ_ABORTP        4
#define LRADJ_ABORTD        8
#define LRADJ_IRQ           4
#define LRADJ_FIRQ          4
#define LRADJ_SWI           4

/* Vector numbers used by assign_handler and the mon_assignhandler()
 * API function...
 */
#define VEC_RST              0
#define VEC_UND              1
#define VEC_SWI              2
#define VEC_ABP              3
#define VEC_ABD              4
#define VEC_IRQ              5
#define VEC_RESERVED         6
#define VEC_FIQ              7

/* Taken from RTEMS score/cpu.h */
#define ARM_EXCEPTION_FRAME_SIZE 80
#define ARM_EXCEPTION_FRAME_REGISTER_SP_OFFSET 52
#define ARM_EXCEPTION_FRAME_VFP_CONTEXT_OFFSET 72

#define ARM_PSR_N (1 << 31)
#define ARM_PSR_Z (1 << 30)
#define ARM_PSR_C (1 << 29)
#define ARM_PSR_V (1 << 28)
#define ARM_PSR_Q (1 << 27)
#define ARM_PSR_J (1 << 24)
#define ARM_PSR_GE_SHIFT 16
#define ARM_PSR_GE_MASK (0xf << ARM_PSR_GE_SHIFT)
#define ARM_PSR_E (1 << 9)
#define ARM_PSR_A (1 << 8)
#define ARM_PSR_I (1 << 7)
#define ARM_PSR_F (1 << 6)
#define ARM_PSR_T (1 << 5)
#define ARM_PSR_M_SHIFT 0
#define ARM_PSR_M_MASK (0x1f << ARM_PSR_M_SHIFT)
#define ARM_PSR_M_USR 0x10
#define ARM_PSR_M_FIQ 0x11
#define ARM_PSR_M_IRQ 0x12
#define ARM_PSR_M_SVC 0x13
#define ARM_PSR_M_ABT 0x17
#define ARM_PSR_M_UND 0x1b
#define ARM_PSR_M_SYS 0x1f

#ifndef _ASSEMBLY_

#include "stddefs.h"

/* Exception context.
 * These data structures gratefully taken from the RTEMS
 * source code cpukit/score/cpu/arm/rtems/score/cpu.h
 */

typedef struct {
  uint32_t register_fpexc;
  uint32_t register_fpscr;
  uint64_t register_d0;
  uint64_t register_d1;
  uint64_t register_d2;
  uint64_t register_d3;
  uint64_t register_d4;
  uint64_t register_d5;
  uint64_t register_d6;
  uint64_t register_d7;
  uint64_t register_d8;
  uint64_t register_d9;
  uint64_t register_d10;
  uint64_t register_d11;
  uint64_t register_d12;
  uint64_t register_d13;
  uint64_t register_d14;
  uint64_t register_d15;
  uint64_t register_d16;
  uint64_t register_d17;
  uint64_t register_d18;
  uint64_t register_d19;
  uint64_t register_d20;
  uint64_t register_d21;
  uint64_t register_d22;
  uint64_t register_d23;
  uint64_t register_d24;
  uint64_t register_d25;
  uint64_t register_d26;
  uint64_t register_d27;
  uint64_t register_d28;
  uint64_t register_d29;
  uint64_t register_d30;
  uint64_t register_d31;
} ARM_VFP_context;

typedef struct {
  uint32_t register_r0;
  uint32_t register_r1;
  uint32_t register_r2;
  uint32_t register_r3;
  uint32_t register_r4;
  uint32_t register_r5;
  uint32_t register_r6;
  uint32_t register_r7;
  uint32_t register_r8;
  uint32_t register_r9;
  uint32_t register_r10;
  uint32_t register_r11;
  uint32_t register_r12;
  uint32_t register_sp;
  void *register_lr;
  void *register_pc;
  uint32_t register_cpsr;
  int vector;
  const ARM_VFP_context *vfp_context;
  uint32_t reserved_for_stack_alignment;
} CPU_Exception_frame;

#endif
