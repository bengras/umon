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
 * FILENAME_HERE
 *
 * DESCRIPTION_HERE
 *
 * Original author:     Ed Sutter (ed.sutter@alcatel-lucent.com)
 *
 */

#include "config.h"
#include "arm.h"
#include "stddefs.h"
#include "genlib.h"
#include "warmstart.h"

ulong   ExceptionAddr;
int ExceptionType;

#define PRIx32 "lx"
#define PRIxPTR "lx"

/* Taken from RTEMS cpukit/score/cpu/arm/arm-exception-frame-print.c */
static void _CPU_Exception_frame_print( const CPU_Exception_frame *frame )
{
  printf(
    "\n"
    "R0   = 0x%08" PRIx32 " R8  = 0x%08" PRIx32 "\n"
    "R1   = 0x%08" PRIx32 " R9  = 0x%08" PRIx32 "\n"
    "R2   = 0x%08" PRIx32 " R10 = 0x%08" PRIx32 "\n"
    "R3   = 0x%08" PRIx32 " R11 = 0x%08" PRIx32 "\n"
    "R4   = 0x%08" PRIx32 " R12 = 0x%08" PRIx32 "\n"
    "R5   = 0x%08" PRIx32 " SP  = 0x%08" PRIx32 "\n"
    "R6   = 0x%08" PRIx32 " LR  = 0x%08" PRIxPTR "\n"
    "R7   = 0x%08" PRIx32 " PC  = 0x%08" PRIxPTR "\n"
    "VEC = 0x%08" PRIxPTR "\n",
    frame->register_r0,
    frame->register_r8,
    frame->register_r1,
    frame->register_r9,
    frame->register_r2,
    frame->register_r10,
    frame->register_r3,
    frame->register_r11,
    frame->register_r4,
    frame->register_r12,
    frame->register_r5,
    frame->register_sp,
    frame->register_r6,
    (uint32_t) frame->register_lr,
    frame->register_r7,
    (uint32_t) frame->register_pc,
    (uint32_t) frame->vector
  );
}

/***********************************************************************
 *
 * umon_exception()
 * Default exception handler used by the low level code in vectors_arm.S.
 */
void
umon_exception(CPU_Exception_frame *frame)
{
    _CPU_Exception_frame_print(frame);

    ExceptionAddr = (uint32_t) frame->register_pc;
    ExceptionType = frame->vector;

    monrestart(EXCEPTION);
}

/***********************************************************************
 *
 * ExceptionType2String():
 * This function simply returns a string that verbosely describes
 * the incoming exception type (vector number).
 */
char *
ExceptionType2String(int type)
{
    char *string;

    switch(type) {
    case EXCTYPE_UNDEF:
        string = "Undefined instruction";
        break;
    case EXCTYPE_ABORTP:
        string = "Abort prefetch";
        break;
    case EXCTYPE_ABORTD:
        string = "Abort data";
        break;
    case EXCTYPE_IRQ:
        string = "IRQ";
        break;
    case EXCTYPE_FIRQ:
        string = "Fast IRQ";
        break;
    case EXCTYPE_NOTASSGN:
        string = "Not assigned";
        break;
    case EXCTYPE_SWI:
        string = "Software Interrupt";
        break;
    default:
        string = "???";
        break;
    }
    return(string);
}

void
vinit(void)
{
}
