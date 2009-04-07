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
 * Portions created by the Initial Developer are Copyright (C) 1993-2006
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

//#define DOPROF

#ifdef DOPROF
#include "../vprof/vprof.h"
#endif

namespace MMgc
{
	const void *GCHashtable::DELETED = (const void*)1;
	
	static const void *EMPTY[2] = { NULL, NULL };

	GCHashtable::GCHashtable(unsigned int capacity, int options) : options(options), table(0), numValues(0), numDeleted(0)
	{
		tableSize = capacity*2;
		if(tableSize > 0)
			grow();
		else {
			tableSize = 4;
			numValues = 4;
			table = EMPTY;
		}
	}

	GCHashtable::~GCHashtable()
	{
		if(options & OPTION_MALLOC) {
			if(table)
				VMPI_free((char*)table);
		}	else {
			delete [] table;
		}
		table = 0;
		tableSize = 0;
		numDeleted = 0;
		numValues = 0;
	}

//Werner's intern table improvement, improves things a little with lots of removes, still doesn't solve completely
//#define CONSUME_DELETED_SLOTS
	// this does solve it completely
	// FIXME: make this a C++ template option so the hashtable consumer can choose linear or quadratic probe
//#define LINEAR_PROBE

	void GCHashtable::put(const void *key, const void *value)
	{
		GCAssert(table != NULL);
		int i = find(key, table, tableSize);
		if (!equals(table[i], key)) {
			// .75 load factor, note we don't take numDeleted into account
			// numValues includes numDeleted
			if(numValues * 8 >= tableSize * 3)
			{
				grow();
				// grow rehashes
				i = find(key, table, tableSize);
			}

#ifdef CONSUME_DELETED_SLOTS
			if(table[i] == DELETED)
				numDeleted--;
#endif
			table[i] = key;
			numValues++;
		}
		table[i+1] = value;
	}
	
	const void *GCHashtable::remove(const void *key)
	{
		const void *ret = NULL;
		int i = find(key, table, tableSize);
		if (table[i] == key) {
#ifdef LINEAR_PROBE
			int bitmask = (tableSize - 1) & ~0x1;
			// if we are the end of the chain we can trim the chain
			if(i  > 0 && table[(i+2) & bitmask] == NULL)
				table[i] = NULL;
			else
#endif
				table[i] = (const void*)DELETED;
			ret = table[i+1];
			table[i+1] = NULL;
			numDeleted++;
			// this helps a bit on pathologic memory profiler use case, needs more investigation
			// 5% deleted == rehash
			//if(numDeleted * 40 >= tableSize)
			//	grow();
		}		
		return ret;
	}

	const void *GCHashtable::get(const void *key)
	{
		return table[find(key, table, tableSize)+1];
	}

	unsigned GCHashtable::equals(const void *k1, const void *k2)
	{
		if(k1 == k2) 
			return true;
		if(k1 && k2 && (options & OPTION_STRINGS))
			return VMPI_strcmp((const char*)k1, (const char*)k2) == 0;
		return false;
	}
	
	unsigned GCHashtable::hash(const void *key)
	{
		if(options & OPTION_STRINGS) {
			unsigned hashCode = 0;
			const char *s = (const char*)key;
			while (*s++) {
				hashCode = (hashCode >> 28) ^ (hashCode << 4) ^ ((uintptr_t)*s << ((uintptr_t)s & 0x3));
			}
			return hashCode;
		}
		return (0x7FFFFFFF & (uintptr_t)key);
	}

	/* static */
	int GCHashtable::find(const void *key, const void **table, unsigned int tableSize)
	{	
		GCAssert(key != (const void*)DELETED);

#ifndef LINEAR_PROBE
		// this is a quadratic probe but we only hit even numbered slots since those hold keys.
		int n = 7 << 1;
#endif

		#if defined(DOPROF) || defined(_DEBUG)
		unsigned loopCount = 0;
		#endif

		int bitmask = (tableSize - 1) & ~0x1;
	    unsigned i = hash(key) & bitmask;  
		const void *k;
#ifdef CONSUME_DELETED_SLOTS
		int firstDeletedSlot = -1;
#endif
    while (equals((k=table[i]), key) == false && k != NULL)
		{
#ifdef CONSUME_DELETED_SLOTS
			if(k == DELETED && firstDeletedSlot == -1)
				firstDeletedSlot = i;
#endif

#ifdef LINEAR_PROBE
			i = (i+2) & bitmask;
#else
			i = (i + (n += 2)) & bitmask;		// quadratic probe
#endif
#if defined(DOPROF) || defined(_DEBUG)
			loopCount++;
#endif
			GCAssert(loopCount < tableSize);			// don't scan forever
		}
#ifdef CONSUME_DELETED_SLOTS
		if(k == NULL && firstDeletedSlot == (int)i) {
			i = firstDeletedSlot;
		}
#endif
#ifdef DOPROF
		_nvprof("loopCount", loopCount);
#endif
		GCAssert(i <= ((tableSize-1)&~0x1));
		return i;
	}

	void GCHashtable::grow()
	{
		int newTableSize = tableSize;

		unsigned int occupiedSlots = numValues - numDeleted;
		GCAssert(numValues >= numDeleted);

		// grow or shrink as appropriate:
		// if we're greater than %50 full grow
		// if we're less than %10 shrink
		// else stay the same
		if (4*occupiedSlots > tableSize)
			newTableSize <<= 1;
		else if(20*occupiedSlots < tableSize && 
				tableSize > kDefaultSize && table)
			newTableSize >>= 1;

		const void **newTable;
		if(options & OPTION_MALLOC) {
			newTable = (const void**)VMPI_alloc(newTableSize*sizeof(void*));
		} else {
			newTable = new const void*[newTableSize];
		}
		VMPI_memset(newTable, 0, newTableSize*sizeof(void*));

		numValues = 0;
		numDeleted = 0;

		if(table)
		{
			for (int i=0, n=tableSize; i < n; i += 2)	
      {
        const void *oldKey;
        if ((oldKey=table[i]) != NULL)
        {
           // inlined & simplified version of put()
					if(oldKey != (const void*)DELETED) {
						int j = find(oldKey, newTable, newTableSize);
						newTable[j] = oldKey;
						newTable[j+1] = table[i+1];
						numValues++;
					}
				}
			}
		}
	
		if(table != EMPTY) {
			if(options & OPTION_MALLOC) {
				if(table)	
					VMPI_free((char*)table);
			} else {
				delete [] table;
			}
		}
		table = newTable;
		tableSize = newTableSize;
		GCAssert(table != NULL);
	}

	int GCHashtable::nextIndex(int index)
	{
		unsigned int i = index<<1;
		while(i < tableSize)
		{
			if(table[i] > DELETED)
				return (i>>1)+1;
			i += 2;
		}
		return 0;
	}
}

