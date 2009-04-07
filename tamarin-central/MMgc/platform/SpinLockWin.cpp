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

#include "MMgc.h"

class SpinLockWin : public MMgc::GCAllocObject
{
public:
	SpinLockWin()
	{
		sl = 0;
	}

	inline bool Acquire()
	{
	#ifdef MMGC_64BIT
		//!!@ fires off for some reason
		//GCAssert(sl==0 || sl==1 || sl==0); // Poor mans defense against thread timing issues, per Tom R.
		while (_InterlockedCompareExchange64(&sl, 1, 0) != 0) {
	#else
		//!!@ fires off for some reason
		//GCAssert(sl==0 || sl==1 || sl==0); // Poor mans defense against thread timing issues, per Tom R.
		while (InterlockedCompareExchange(&sl, 1, 0) != 0) {
	#endif
			Sleep(0);
		}

		return true;
	}
	
	inline bool Release()
	{
	#ifdef MMGC_64BIT
		_InterlockedExchange64(&sl, 0);
	#else
		InterlockedExchange(&sl, 0);
	#endif

		return true;
	}

private:
#ifdef MMGC_64BIT
	volatile LONG64 sl;
#else
	long sl;
#endif
};

vmpi_spin_lock_t VMPI_lockCreate()
{
	return (vmpi_spin_lock_t) (new SpinLockWin);
}

void VMPI_lockDestroy(vmpi_spin_lock_t lock)
{
	delete (SpinLockWin*)lock;
}

bool VMPI_lockAcquire(vmpi_spin_lock_t lock)
{
	return ((SpinLockWin*)lock)->Acquire();
}

bool VMPI_lockRelease(vmpi_spin_lock_t lock)
{
	return ((SpinLockWin*)lock)->Release();
}
