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
 * vectors_arm.S
 *
 * Minimalist exception handlers...
 * Catch the exception, call generic handler with exception-specific id.
 *
 * Original author:     Ed Sutter (ed.sutter@alcatel-lucent.com)
 *
 */

#define _ASSEMBLY_ 1

#include "arm.h"

#define MORE_CONTEXT_SIZE \
  (ARM_EXCEPTION_FRAME_SIZE - ARM_EXCEPTION_FRAME_REGISTER_SP_OFFSET)

#define EXCEPTIONSAVE(n)         \
	sub     sp, #MORE_CONTEXT_SIZE	 ; \
        stmdb   sp!, {r0-r12}	 ; \
        mov     r4, #(n)	 ; \
	 ; \
        b       save_more_context	 ; \

.global undefined_instruction
.global software_interrupt
.global abort_prefetch
.global abort_data
.global not_assigned
.global interrupt_request
.global fast_interrupt_request

undefined_instruction:
EXCEPTIONSAVE(EXCTYPE_UNDEF)

software_interrupt:
EXCEPTIONSAVE(EXCTYPE_SWI)

abort_prefetch:
EXCEPTIONSAVE(EXCTYPE_ABORTP)

abort_data:
EXCEPTIONSAVE(EXCTYPE_ABORTD)

not_assigned:
EXCEPTIONSAVE(EXCTYPE_NOTASSGN)

interrupt_request:
EXCEPTIONSAVE(EXCTYPE_IRQ)

fast_interrupt_request:
EXCEPTIONSAVE(EXCTYPE_FIRQ)

/* This code gratefully taken from RTEMS */

save_more_context:
        /* Save more context */
        mov     r2, lr
        mrs     r3, spsr
        mrs     r7, cpsr
        orr     r5, r3, #ARM_PSR_I
        bic     r5, #ARM_PSR_T
        msr     cpsr, r5
        mov     r0, sp
        mov     r1, lr
        msr     cpsr, r7
        mov     r5, #0
        add     r6, sp, #ARM_EXCEPTION_FRAME_REGISTER_SP_OFFSET
        stm     r6, {r0-r5}

        /* Argument for high level handler */
        mov     r0, sp

        /* Clear VFP context pointer */
        add     r3, sp, #ARM_EXCEPTION_FRAME_VFP_CONTEXT_OFFSET
        mov     r1, #0
        str     r1, [r3]

        /* Call high level handler */
	b umon_exception

        /* Just in case */
twiddle:
        b       twiddle

