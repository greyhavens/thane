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

#ifndef __FixedAlloc__
#define __FixedAlloc__

namespace MMgc
{
	/**
	 * This is a fast, fixed-size memory allocator for manually freed
	 * objects.
	 *
	 * Memory is allocated from the system in 4096 byte chunks
	 * via the GCHeap class.  Not that this won't work well for
	 * large objects (>400 bytes).  For that, we have the
	 * FixedAllocLarge allocator which will allocate multiple
	 * pages at a time to minimize waste.
	 */
	class MMGC_API FixedAlloc : public GCAllocObject
	{
		friend class FixedMalloc;
		friend class FastAllocator;
		friend class GC;
	public:
		FixedAlloc(int itemSize, GCHeap* heap);
		~FixedAlloc();

		void* Alloc(size_t size);
		static void Free(void *item);

		size_t GetBytesInUse();

		size_t GetItemSize() const;
		int GetMaxAlloc() const { return m_maxAlloc; }

		size_t GetNumChunks() { return m_maxAlloc / m_itemsPerBlock; }	

		static FixedAlloc *GetFixedAlloc(void *item)
		{
			return GetFixedBlock(item)->alloc;
		}

	private:

		struct FixedBlock
		{
			void*  firstFree;        // first item on free list
			void*  nextItem;         // next free item
			FixedBlock* next;
			FixedBlock* prev;
			uint16_t numAlloc;
			uint16_t size;
			FixedBlock *prevFree;
			FixedBlock *nextFree;
			FixedAlloc *alloc;
			char   items[1];
		};

		GCHeap *m_heap;
		unsigned int    m_itemsPerBlock;
		size_t    m_itemSize;

		// The list of chunk blocks
		FixedBlock* m_firstBlock; 
		FixedBlock* m_lastBlock;

		// The lowest priority block that has free items		
		FixedBlock* m_firstFree;

		int    m_maxAlloc;

		bool IsFull(FixedBlock *b) const { return b->numAlloc == m_itemsPerBlock; }
		FixedBlock* CreateChunk();
		void FreeChunk(FixedBlock* b);

		static inline FixedBlock *GetFixedBlock(const void *item)
		{
			return (FixedBlock*) ((uintptr_t)item & ~0xFFF);
		}

		static inline size_t Size(const void *item)
		{
			return GetFixedBlock(item)->size;
		}
	};

	class FixedAllocSafe : public FixedAlloc
	{
	public:
		FixedAllocSafe(int itemSize, GCHeap* heap) : FixedAlloc(itemSize, heap) 
		{
			m_spinlock = VMPI_lockCreate();
			GCAssert(m_spinlock != NULL);
		}
		
		~FixedAllocSafe()
		{
			VMPI_lockDestroy(m_spinlock);
		}

		void* Alloc(size_t size)
		{
			MMGC_LOCK(m_spinlock);
			return FixedAlloc::Alloc(size); 
		}

		void Free(void *ptr)
		{
			MMGC_LOCK(m_spinlock);
			FixedAlloc::Free(ptr);
		}

		static FixedAllocSafe *GetFixedAllocSafe(void *item)
		{
			return (FixedAllocSafe*) FixedAlloc::GetFixedAlloc(item);
		}

	private:

#ifdef MMGC_LOCKING
		vmpi_spin_lock_t m_spinlock;
#endif
	};

	/**
	 * classes that need fast lock free allocation should subclass this and pass
	 * a FixedAlloc * to the new parameter.  One new/delete are lock free, scalar
	 * allocations use the normal locked general size allocator.
	 */
	class FastAllocator 
	{		
	public:
		static void *operator new(size_t size, FixedAlloc *alloc)
		{
			return alloc->Alloc(size);
		}
		
		static void operator delete (void *item)
		{
			FixedAlloc::Free(item);
		}

		// allow array allocation  as well		
		static void *operator new[](size_t size);		
		static void operator delete [](void *item);
	};
}

#endif /* __FixedAlloc__ */
