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
 * cf.h:
 *
 * Header file supporting compact flash command.
 *
 * Original author:     Ed Sutter (ed.sutter@alcatel-lucent.com)
 *
 */

#ifndef CF_BLKSIZE
#define CF_BLKSIZE  512
#endif


/* These two functions must be supplied by the port-specific code.
 */
int cfInit(int interface,int verbose);
int cfRead(int interface,char *buf,int bknum,int bkcnt,int verbose);
int cfWrite(int interface,char *buf,int bknum,int bkcnt,int verbose);

