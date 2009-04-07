/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *   leon.sha@sun.com
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __MMgc__
#define __MMgc__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef NEED_MMGC_CONFIG_H
#include "MMgc-config.h"
#endif

#if defined(MMGC_CUSTOM_BUILD)
    #include "MMgcCustomBuild.h"
#else
	#ifdef WIN32
		#ifdef ARM
			#include "armbuild.h"
		#else
			#include "winbuild.h"
		#endif
	#endif

	#ifdef _MAC
		#include "macbuild.h"
	#endif

	#ifdef UNIX
		#include "unixbuild.h"
	#endif

	// don't include armbuild.h when MMGC_CUSTOM_BUILD is used
	#ifdef MMGC_ARM
		#include "armbuild.h"
	#endif
#endif

#include "VMPI.h"

#if defined(WIN32) && defined(MMGC_64BIT)
#include <setjmpex.h>
#else
#include <setjmp.h>
#endif

#if defined(SCRIPT_DEBUGGER) || defined(DEBUGGER)
#define AVMPLUS_SAMPLER
#endif

#ifdef MMGC_AVMPLUS
#  define MMGC_RCROOT_SUPPORT
#endif

#ifndef _MSC_VER
#define __forceinline
#else
#ifndef DEBUG
#include <memory.h>
#include <string.h>
#pragma intrinsic(memcmp)
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(strlen)
#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#endif // DEBUG
#endif

#include "GCDebug.h"
#include "GCLog.h"

/*
 * If _GCHeapLock is defined, a spin lock is used for thread safety
 * on all public API's (Alloc, Free, ExpandHeap)
 *
 * Warning:
 * We may use GCHeap for allocation on other threads, hence the
 * spinlock, but the MMgc garbage collector in general is not
 * thread-safe.
 */

#ifdef MMGC_LOCKING
#define MMGC_LOCK(_x) GCAcquireSpinlock _lock(_x)
#include "GCSpinLock.h"
#else
#define MMGC_LOCK(_x) 
#endif

namespace MMgc
{
	class GC;
	class RCObject;
	class GCWeakRef;
	class GCObject;
	class GCHashtable;
	class Cleaner;
	class GCAlloc;
	class GCHeap;
}

#include "GCTypes.h"
#include "OOM.h"
#include "GCStack.h"
#include "GCThreads.h"
#include "GCAllocObject.h"
#include "GCHashtable.h"
#include "GCMemoryProfiler.h"
#include "GCThreadLocal.h"
#include "FixedAlloc.h"
#include "FixedMalloc.h"
#include "GCHeap.h"
#include "GCAlloc.h"
#include "GCLargeAlloc.h"
#include "GCGlobalNew.h"
#include "GC.h"
#include "GCObject.h"
#include "GCWeakRef.h"
#include "WriteBarrier.h"

// remove these when the player stops using it
#define MMGC_DRC
#define WRITE_BARRIERS

#endif /* __MMgc__ */

