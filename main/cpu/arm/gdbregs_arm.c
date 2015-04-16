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
 * gdbregs_arm.c
 *
 * List of the register names for the ARM in the order they
 * are expected by the GDB 'g' command (read all registers).
 * R13=SP, R14=LR
 *
 * Original author:     Ed Sutter (ed.sutter@alcatel-lucent.com)
 *
 */
#define CPU_PC_REG	"PC"

static char  *gdb_regtbl[] = {
	"R0",	"R1",	"R2",	"R3", "R4", "R5", "R6", "R7",
	"R8",	"R9",	"R10",	"R11", "R12", "R13", "R14", "PC",
	"R0",	"CPSR",	
};
