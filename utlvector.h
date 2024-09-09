//#pragma once
//
//template< class T, class I = int >
//class CUtlMemory {
//public:
//	__forceinline T& operator[ ](I i) {
//		return m_pMemory[i];
//	}
//
//	static int CalcNewAllocationCount(int count, int size, int requested, int bytes) {
//		if (size)
//			count = ((1 + ((requested - 1) / size)) * size);
//
//		else {
//			if (!count)
//				count = (31 + bytes) / bytes;
//
//			while (count < requested)
//				count *= 2;
//		}
//
//		return count;
//	}
//
//	__forceinline void Grow(int count = 1) {
//		if (IsExternallyAllocated())
//			return;
//
//		int requested = m_nAllocationCount + count;
//		int new_alloc_count = CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, requested, sizeof(T));
//
//		if ((int)(I)new_alloc_count < requested) {
//
//			if ((int)(I)new_alloc_count == 0 && (int)(I)(new_alloc_count - 1) >= requested)
//				--new_alloc_count;
//
//			else {
//				if ((int)(I)requested != requested)
//					return;
//
//				while ((int)(I)new_alloc_count < requested)
//					new_alloc_count = (new_alloc_count + requested) / 2;
//			}
//		}
//
//		m_nAllocationCount = new_alloc_count;
//
//		if (m_pMemory)
//			m_pMemory = (T*)g_pMemAlloc->Realloc(m_pMemory, m_nAllocationCount * sizeof(T));
//
//		else
//			m_pMemory = (T*)g_pMemAlloc->Alloc(m_nAllocationCount * sizeof(T));
//	}
//
//	__forceinline bool IsExternallyAllocated() const {
//		return m_nGrowSize < 0;
//	}
//
//protected:
//	T* m_pMemory;
//	int m_nAllocationCount;
//	int m_nGrowSize;
//};
//
//template < class T >
//__forceinline T* Construct(T* pMemory) {
//	return ::new(pMemory) T;
//}
//
//template <class T>
//__forceinline void Destruct(T* pMemory) {
//	pMemory->~T();
//}
//
//template< class T, class A = CUtlMemory< T > >
//class CUtlVector {
//	typedef A CAllocator;
//public:
//	__forceinline T& operator[](int i) {
//		return m_Memory[i];
//	}
//
//	__forceinline T& Element(int i) {
//		return m_Memory[i];
//	}
//
//	__forceinline T* Base() {
//		return m_Memory.Base();
//	}
//
//	__forceinline int Count() const {
//		return m_Size;
//	}
//
//	__forceinline void RemoveAll() {
//		for (int i = m_Size; --i >= 0; )
//			Destruct(&Element(i));
//
//		m_Size = 0;
//	}
//
//	__forceinline int AddToTail() {
//		return InsertBefore(m_Size);
//	}
//
//	__forceinline int InsertBefore(int elem) {
//		GrowVector();
//		ShiftElementsRight(elem);
//		Construct(&Element(elem));
//
//		return elem;
//	}
//
//protected:
//	__forceinline void GrowVector(int num = 1) {
//		if (m_Size + num > m_Memory.NumAllocated())
//			m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());
//
//		m_Size += num;
//		ResetDbgInfo();
//	}
//
//	__forceinline void ShiftElementsRight(int elem, int num = 1) {
//		int numToMove = m_Size - elem - num;
//		if ((numToMove > 0) && (num > 0))
//			memmove(&Element(elem + num), &Element(elem), numToMove * sizeof(T));
//	}
//
//	CAllocator m_Memory;
//	int        m_Size;
//	T* m_pElements;
//
//	__forceinline void ResetDbgInfo() {
//		m_pElements = Base();
//	}
//};
//
//inline int UtlMemory_CalcNewAllocationcount(int nAllocationcount, int nGrowSize, int nNewSize, int nBytesItem) {
//	if (nGrowSize) {
//		nAllocationcount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
//	}
//	else {
//		if (!nAllocationcount) {
//			// Compute an allocation which is at least as big as a cache line...
//			nAllocationcount = (31 + nBytesItem) / nBytesItem;
//		}
//
//		while (nAllocationcount < nNewSize) {
//			nAllocationcount *= 2;
//		}
//	}
//
//	return nAllocationcount;
//}
//
//template< class T, class I = int >
//class c_utl_memory {
//public:
//	bool is_valid_index(I i) const {
//		long x = i;
//		return (x >= 0) && (x < m_alloc_count);
//	}
//
//	T& operator[](I i);
//	const T& operator[](I i) const;
//
//	T* base() {
//		return m_memory;
//	}
//
//	int num_allocated() const {
//		return m_alloc_count;
//	}
//
//	void grow(int num) {
//		assert(num > 0);
//
//		auto old_alloc_count = m_alloc_count;
//		// Make sure we have at least numallocated + num allocations.
//		// Use the grow rules specified for this memory (in m_grow_size)
//		int alloc_requested = m_alloc_count + num;
//
//		int new_alloc_count = UtlMemory_CalcNewAllocationcount(m_alloc_count, m_grow_size, alloc_requested, sizeof(T));
//
//		// if m_alloc_requested wraps index type I, recalculate
//		if ((int)(I)new_alloc_count < alloc_requested) {
//			if ((int)(I)new_alloc_count == 0 && (int)(I)(new_alloc_count - 1) >= alloc_requested) {
//				--new_alloc_count; // deal w/ the common case of m_alloc_count == MAX_USHORT + 1
//			}
//			else {
//				if ((int)(I)alloc_requested != alloc_requested) {
//					// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
//					assert(0);
//					return;
//				}
//				while ((int)(I)new_alloc_count < alloc_requested) {
//					new_alloc_count = (new_alloc_count + alloc_requested) / 2;
//				}
//			}
//		}
//
//		m_alloc_count = new_alloc_count;
//
//		if (m_memory) {
//			auto ptr = new unsigned char[m_alloc_count * sizeof(T)];
//
//			memcpy(ptr, m_memory, old_alloc_count * sizeof(T));
//			m_memory = (T*)ptr;
//		}
//		else {
//			m_memory = (T*)new unsigned char[m_alloc_count * sizeof(T)];
//		}
//	}
//
//protected:
//	T* m_memory;
//	int m_alloc_count;
//	int m_grow_size;
//};
//
//template< class T, class I >
//T& c_utl_memory< T, I >::operator[](I i) {
//	assert(is_valid_index(i));
//	return m_memory[i];
//}
//
//template< class T, class I >
//const T& c_utl_memory< T, I >::operator[](I i) const {
//	assert(is_valid_index(i));
//	return m_memory[i];
//}
//
//template< class T >
//void destruct(T* memory) {
//	memory->~T();
//}
//
//template< class T >
//T* construct(T* memory) {
//	return ::new(memory) T;
//}
//
//template< class T >
//T* copy_construct(T* memory, T const& src) {
//	return ::new(memory) T(src);
//}
//
//template< class T, class A = c_utl_memory< T > >
//class c_utl_vector {
//	typedef A c_allocator;
//
//	typedef T* iterator;
//	typedef const T* const_iterator;
//public:
//	T& operator[](int i);
//	const T& operator[](int i) const;
//
//	T& element(int i) {
//		return m_memory[i];
//	}
//
//	const T& element(int i) const {
//		assert(is_valid_index(i));
//		return m_memory[i];
//	}
//
//	T* base() {
//		return m_memory.base();
//	}
//
//	int count() const {
//		return m_size;
//	}
//
//	void remove_all() {
//		for (int i = m_size; --i >= 0; )
//			destruct(&element(i));
//
//		m_size = 0;
//	}
//
//	bool is_valid_index(int i) const {
//		return (i >= 0) && (i < m_size);
//	}
//
//	void shift_elements_right(int elem, int num = 1) {
//		assert(is_valid_index(elem) || (m_size == 0) || (num == 0));
//		int num_to_move = m_size - elem - num;
//		if ((num_to_move > 0) && (num > 0))
//			memmove(&element(elem + num), &element(elem), num_to_move * sizeof(T));
//	}
//
//	void shift_elements_left(int elem, int num = 1) {
//		assert(is_valid_index(elem) || (m_size == 0) || (num == 0));
//		int numToMove = m_size - elem - num;
//		if ((numToMove > 0) && (num > 0))
//			memmove(&element(elem), &element(elem + num), numToMove * sizeof(T));
//	}
//
//	void grow_vector(int num = 1) {
//		if (m_size + num > m_memory.num_allocated()) {
//			m_memory.grow(m_size + num - m_memory.num_allocated());
//		}
//
//		m_size += num;
//	}
//
//	int insert_before(int elem) {
//		// Can insert at the end
//		assert((elem == count()) || is_valid_index(elem));
//
//		grow_vector();
//		shift_elements_right(elem);
//		construct(&element(elem));
//		return elem;
//	}
//
//	int add_to_head() {
//		return insert_before(0);
//	}
//
//	int add_to_tail() {
//		return insert_before(m_size);
//	}
//
//	int insert_before(int elem, const T& src) {
//		// Can't insert something that's in the list... reallocation may hose us
//		assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
//
//		// Can insert at the end
//		assert((elem == count()) || is_valid_index(elem));
//
//		grow_vector();
//		shift_elements_right(elem);
//		copy_construct(&element(elem), src);
//		return elem;
//	}
//
//	int add_to_tail(const T& src) {
//		// Can't insert something that's in the list... reallocation may hose us
//		return insert_before(m_size, src);
//	}
//
//	int find(const T& src) const {
//		for (int i = 0; i < count(); ++i) {
//			if (element(i) == src)
//				return i;
//		}
//		return -1;
//	}
//
//	void remove(int elem) {
//		destruct(&element(elem));
//		shift_elements_left(elem);
//		--m_size;
//	}
//
//	bool find_and_remove(const T& src) {
//		int elem = find(src);
//		if (elem != -1) {
//			remove(elem);
//			return true;
//		}
//		return false;
//	}
//
//	iterator begin() {
//		return base();
//	}
//
//	const_iterator begin() const {
//		return base();
//	}
//
//	iterator end() {
//		return base() + count();
//	}
//
//	const_iterator end() const {
//		return base() + count();
//	}
//
//protected:
//	c_allocator m_memory;
//	int m_size;
//	T* m_elements;
//};
//
//template< typename T, class A >
//T& c_utl_vector< T, A >::operator[](int i) {
//	assert(i < m_size);
//	return m_memory[i];
//}
//
//template< typename T, class A >
//const T& c_utl_vector< T, A >::operator[](int i) const {
//	assert(i < m_size);
//	return m_memory[i];
//}
//
//
#pragma once

template <class T>
inline void Destruct(T* pMemory) {
	pMemory->~T();
}

template < class T, class I = int >
class CUtlMemory {
public:
	// constructor, destructor
	CUtlMemory(int nGrowSize = 0, int nInitSize = 0);
	CUtlMemory(T* pMemory, int numElements);
	CUtlMemory(const T* pMemory, int numElements);
	~CUtlMemory();

	// Set the size by which the memory grows
	void Init(int nGrowSize = 0, int nInitSize = 0);

	class Iterator_t {
	public:
		Iterator_t(I i) :
			index(i) {
		}
		I index;

		bool operator==(const Iterator_t it) const { return index == it.index; }
		bool operator!=(const Iterator_t it) const { return index != it.index; }
	};
	Iterator_t First() const { return Iterator_t(IsIdxValid(0) ? 0 : InvalidIndex()); }
	Iterator_t Next(const Iterator_t& it) const { return Iterator_t(IsIdxValid(it.index + 1) ? it.index + 1 : InvalidIndex()); }
	I GetIndex(const Iterator_t& it) const { return it.index; }
	bool IsIdxAfter(I i, const Iterator_t& it) const { return i > it.index; }
	bool IsValidIterator(const Iterator_t& it) const { return IsIdxValid(it.index); }
	Iterator_t InvalidIterator() const { return Iterator_t(InvalidIndex()); }

	// element access
	T& operator[](I i);
	const T& operator[](I i) const;
	T& Element(I i);
	const T& Element(I i) const;

	bool IsIdxValid(I i) const;

	static const I INVALID_INDEX = (I)-1; // For use with COMPILE_TIME_ASSERT
	static I InvalidIndex() { return INVALID_INDEX; }

	T* Base();
	const T* Base() const;

	void SetExternalBuffer(T* pMemory, int numElements);
	void SetExternalBuffer(const T* pMemory, int numElements);
	void AssumeMemory(T* pMemory, int nSize);
	T* Detach();
	void* DetachMemory();

	void Swap(CUtlMemory< T, I >& mem);
	void ConvertToGrowableMemory(int nGrowSize);
	int NumAllocated() const;
	int Count() const;
	void Grow(int num = 1);
	void EnsureCapacity(int num);
	void Purge();
	void Purge(int numElements);
	bool IsExternallyAllocated() const;
	bool IsReadOnly() const;
	void SetGrowSize(int size);

protected:
	void ValidateGrowSize() {
	}

	enum {
		EXTERNAL_BUFFER_MARKER = -1,
		EXTERNAL_CONST_BUFFER_MARKER = -2,
	};

public:
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};


//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------

template < class T, class I >
CUtlMemory< T, I >::CUtlMemory(int nGrowSize, int nInitAllocationCount) :
	m_pMemory(0),
	m_nAllocationCount(nInitAllocationCount), m_nGrowSize(nGrowSize) {
	ValidateGrowSize();
	assert(nGrowSize >= 0);
	if (m_nAllocationCount) {
		m_pMemory = (T*)g_csgo.m_mem_alloc->Alloc(m_nAllocationCount * sizeof(T));
		//m_pMemory = (T*)m_pMemAlloc->Alloc(m_nAllocationCount * sizeof(T));
	}
}

template < class T, class I >
CUtlMemory< T, I >::CUtlMemory(T* pMemory, int numElements) :
	m_pMemory(pMemory),
	m_nAllocationCount(numElements) {
	// Special marker indicating externally supplied modifyable memory
	m_nGrowSize = EXTERNAL_BUFFER_MARKER;
}

template < class T, class I >
CUtlMemory< T, I >::CUtlMemory(const T* pMemory, int numElements) :
	m_pMemory((T*)pMemory),
	m_nAllocationCount(numElements) {
	// Special marker indicating externally supplied modifyable memory
	m_nGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template < class T, class I >
CUtlMemory< T, I >::~CUtlMemory() {
	Purge();
}

template < class T, class I >
void CUtlMemory< T, I >::Init(int nGrowSize /*= 0*/, int nInitSize /*= 0*/) {
	Purge();

	m_nGrowSize = nGrowSize;
	m_nAllocationCount = nInitSize;
	ValidateGrowSize();
	assert(nGrowSize >= 0);
	if (m_nAllocationCount) {
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)g_csgo.m_mem_alloc->Alloc(m_nAllocationCount * sizeof(T));
	}
}

//-----------------------------------------------------------------------------
// Fast swap
//-----------------------------------------------------------------------------
template < class T, class I >
void CUtlMemory< T, I >::Swap(CUtlMemory< T, I >& mem) {
	V_swap(m_nGrowSize, mem.m_nGrowSize);
	V_swap(m_pMemory, mem.m_pMemory);
	V_swap(m_nAllocationCount, mem.m_nAllocationCount);
}

//-----------------------------------------------------------------------------
// Switches the buffer from an external memory buffer to a reallocatable buffer
//-----------------------------------------------------------------------------
template < class T, class I >
void CUtlMemory< T, I >::ConvertToGrowableMemory(int nGrowSize) {
	if (!IsExternallyAllocated())
		return;

	m_nGrowSize = nGrowSize;
	if (m_nAllocationCount) {
		int nNumBytes = m_nAllocationCount * sizeof(T);
		T* pMemory = (T*)MemAlloc_Alloc(nNumBytes);
		memcpy(pMemory, m_pMemory, nNumBytes);
		m_pMemory = pMemory;
	}
	else {
		m_pMemory = NULL;
	}
}

//-----------------------------------------------------------------------------
// Attaches the buffer to external memory....
//-----------------------------------------------------------------------------
template < class T, class I >
void CUtlMemory< T, I >::SetExternalBuffer(T* pMemory, int numElements) {
	// Blow away any existing allocated memory
	Purge();

	m_pMemory = pMemory;
	m_nAllocationCount = numElements;

	// Indicate that we don't own the memory
	m_nGrowSize = EXTERNAL_BUFFER_MARKER;
}

template < class T, class I >
void CUtlMemory< T, I >::SetExternalBuffer(const T* pMemory, int numElements) {
	// Blow away any existing allocated memory
	Purge();

	m_pMemory = const_cast<T*>(pMemory);
	m_nAllocationCount = numElements;

	// Indicate that we don't own the memory
	m_nGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template < class T, class I >
void CUtlMemory< T, I >::AssumeMemory(T* pMemory, int numElements) {
	// Blow away any existing allocated memory
	Purge();

	// Simply take the pointer but don't mark us as external
	m_pMemory = pMemory;
	m_nAllocationCount = numElements;
}

template < class T, class I >
void* CUtlMemory< T, I >::DetachMemory() {
	if (IsExternallyAllocated())
		return NULL;

	void* pMemory = m_pMemory;
	m_pMemory = 0;
	m_nAllocationCount = 0;
	return pMemory;
}

template < class T, class I >
inline T* CUtlMemory< T, I >::Detach() {
	return (T*)DetachMemory();
}

//-----------------------------------------------------------------------------
// element access
//-----------------------------------------------------------------------------
template < class T, class I >
inline T& CUtlMemory< T, I >::operator[](I i) {
	assert(!IsReadOnly());
	// men 
	// assert( IsIdxValid( i ) );
	return m_pMemory[i];
}

template < class T, class I >
inline const T& CUtlMemory< T, I >::operator[](I i) const {
	// can you
	// assert( IsIdxValid( i ) );
	return m_pMemory[i];
}

template < class T, class I >
inline T& CUtlMemory< T, I >::Element(I i) {
	assert(!IsReadOnly());
	// FUCK
	// assert( IsIdxValid( i ) );
	return m_pMemory[i];
}

template < class T, class I >
inline const T& CUtlMemory< T, I >::Element(I i) const {
	// my balls ?
	// assert( IsIdxValid( i ) );
	return m_pMemory[i];
}

//-----------------------------------------------------------------------------
// is the memory externally allocated?
//-----------------------------------------------------------------------------
template < class T, class I >
bool CUtlMemory< T, I >::IsExternallyAllocated() const {
	return (m_nGrowSize < 0);
}

//-----------------------------------------------------------------------------
// is the memory read only?
//-----------------------------------------------------------------------------
template < class T, class I >
bool CUtlMemory< T, I >::IsReadOnly() const {
	return (m_nGrowSize == EXTERNAL_CONST_BUFFER_MARKER);
}

template < class T, class I >
void CUtlMemory< T, I >::SetGrowSize(int nSize) {
	assert(!IsExternallyAllocated());
	assert(nSize >= 0);
	m_nGrowSize = nSize;
	ValidateGrowSize();
}

//-----------------------------------------------------------------------------
// Gets the base address (can change when adding elements!)
//-----------------------------------------------------------------------------
template < class T, class I >
inline T* CUtlMemory< T, I >::Base() {
	assert(!IsReadOnly());
	return m_pMemory;
}

template < class T, class I >
inline const T* CUtlMemory< T, I >::Base() const {
	return m_pMemory;
}

//-----------------------------------------------------------------------------
// Size
//-----------------------------------------------------------------------------
template < class T, class I >
inline int CUtlMemory< T, I >::NumAllocated() const {
	return m_nAllocationCount;
}

template < class T, class I >
inline int CUtlMemory< T, I >::Count() const {
	return m_nAllocationCount;
}

//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
template < class T, class I >
inline bool CUtlMemory< T, I >::IsIdxValid(I i) const {
	// GCC warns if I is an unsigned type and we do a ">= 0" against it (since the comparison is always 0).
	// We Get the warning even if we cast inside the expression. It only goes away if we assign to another variable.
	long x = i;
	return (x >= 0) && (x < m_nAllocationCount);
}

//-----------------------------------------------------------------------------
// Grows the memory
//-----------------------------------------------------------------------------
inline int UtlMemory_CalcNewAllocationCount(int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem) {
	if (nGrowSize) {
		nAllocationCount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
	}
	else {
		if (!nAllocationCount) {
			// Compute an allocation which is at least as big as a cache line...
			nAllocationCount = (31 + nBytesItem) / nBytesItem;
		}

		while (nAllocationCount < nNewSize) {
#ifndef _X360
			nAllocationCount *= 2;
#else
			int nNewAllocationCount = (nAllocationCount * 9) / 8; // 12.5 %
			if (nNewAllocationCount > nAllocationCount)
				nAllocationCount = nNewAllocationCount;
			else
				nAllocationCount *= 2;
#endif
		}
	}

	return nAllocationCount;
}

template < class T, class I >
void CUtlMemory< T, I >::Grow(int num) {
	assert(num > 0);

	if (IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated
		assert(0);
		return;
	}

	auto oldAllocationCount = m_nAllocationCount;
	// Make sure we have at least numallocated + num allocations.
	// Use the grow rules specified for this memory (in m_nGrowSize)
	int nAllocationRequested = m_nAllocationCount + num;

	int nNewAllocationCount = UtlMemory_CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

	// if m_nAllocationRequested wraps index type I, recalculate
	if ((int)(I)nNewAllocationCount < nAllocationRequested) {
		if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested) {
			--nNewAllocationCount; // deal w/ the common case of m_nAllocationCount == MAX_USHORT + 1
		}
		else {
			if ((int)(I)nAllocationRequested != nAllocationRequested) {
				// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
				assert(0);
				return;
			}
			while ((int)(I)nNewAllocationCount < nAllocationRequested) {
				nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
			}
		}
	}

	m_nAllocationCount = nNewAllocationCount;

	if (m_pMemory) {
		auto ptr = (T*)MemAlloc_Alloc(m_nAllocationCount * sizeof(T));

		memcpy(ptr,
			m_pMemory,
			oldAllocationCount * sizeof(T));
		m_pMemory = (T*)ptr;
	}
	else {
		m_pMemory = (T*)MemAlloc_Alloc(m_nAllocationCount * sizeof(T));
	}
}

//-----------------------------------------------------------------------------
// Makes sure we've got at least this much memory
//-----------------------------------------------------------------------------
template < class T, class I >
inline void CUtlMemory< T, I >::EnsureCapacity(int num) {
	if (m_nAllocationCount >= num)
		return;

	if (IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated
		assert(0);
		return;
	}
	m_nAllocationCount = num;

	if (m_pMemory) {
		m_pMemory = (T*)g_csgo.m_mem_alloc->Realloc(m_pMemory, m_nAllocationCount * sizeof(T));
	}
	else {
		m_pMemory = (T*)g_csgo.m_mem_alloc->Alloc(m_nAllocationCount * sizeof(T));
	}
}

//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------
template < class T, class I >
void CUtlMemory< T, I >::Purge() {
	if (!IsExternallyAllocated()) {
		if (m_pMemory) {
			g_csgo.m_mem_alloc->Free((void*)m_pMemory);
			m_pMemory = 0;
		}
		m_nAllocationCount = 0;
	}
}

template < class T, class I >
void CUtlMemory< T, I >::Purge(int numElements) {
	assert(numElements >= 0);

	if (numElements > m_nAllocationCount) {
		// Ensure this isn't a grow request in disguise.
		assert(numElements <= m_nAllocationCount);
		return;
	}

	// If we have zero elements, simply do a purge:
	if (numElements == 0) {
		Purge();
		return;
	}

	if (IsExternallyAllocated()) {
		// Can't shrink a buffer whose memory was externally allocated, fail silently like purge
		return;
	}

	// If the number of elements is the same as the allocation count, we are done.
	if (numElements == m_nAllocationCount) {
		return;
	}

	if (!m_pMemory) {
		// Allocation count is non zero, but memory is null.
		assert(m_pMemory);
		return;
	}
	m_nAllocationCount = numElements;
	m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
}


template< class T, class A = CUtlMemory< T > >
class CUtlVector {
	typedef A CAllocator;

	typedef T* iterator;
	typedef const T* const_iterator;
public:
	T& operator[](int i);
	const T& operator[](int i) const;

	T& Element(int i)
	{
		return m_Memory[i];
	}

	const T& Element(int i) const
	{
		//assert( IsValidIndex( i ) );
		return m_Memory[i];
	}

	T* Base()
	{
		return m_Memory.Base();
	}

	int Count() const
	{
		return m_Size;
	}

	void RemoveAll()
	{
		for (int i = m_Size; --i >= 0; )
			Destruct(&Element(i));

		m_Size = 0;
	}

	bool IsValidIndex(int i) const
	{
		return (i >= 0) && (i < m_Size);
	}

	void ShiftElementsRight(int elem, int num = 1)
	{
		//assert( IsValidIndex( elem ) || ( m_Size == 0 ) || ( num == 0 ) );
		int numToMove = m_Size - elem - num;
		if ((numToMove > 0) && (num > 0))
		{
			memmove(&Element(elem + num), &Element(elem), numToMove * sizeof(T));
		}
	}

	void ShiftElementsLeft(int elem, int num = 1)
	{
		//assert( IsValidIndex( elem ) || ( m_Size == 0 ) || ( num == 0 ) );
		int numToMove = m_Size - elem - num;
		if ((numToMove > 0) && (num > 0))
		{
			memmove(&Element(elem), &Element(elem + num), numToMove * sizeof(T));
		}
	}

	void GrowVector(int num = 1)
	{
		if (m_Size + num > m_Memory.NumAllocated())
		{
			m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());
		}

		m_Size += num;
	}

	int InsertBefore(int elem)
	{
		// Can insert at the end
		//assert( ( elem == Count( ) ) || IsValidIndex( elem ) );

		GrowVector();
		ShiftElementsRight(elem);
		Construct(&Element(elem));
		return elem;
	}

	int AddToHead()
	{
		return InsertBefore(0);
	}

	int AddToTail() {
		return InsertBefore(m_Size);
	}

	int InsertBefore(int elem, const T& src)
	{
		// Can't insert something that's in the list... reallocation may hose us
		assert((Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count())));

		// Can insert at the end
		//assert( ( elem == Count( ) ) || IsValidIndex( elem ) );

		GrowVector();
		ShiftElementsRight(elem);
		CopyConstruct(&Element(elem), src);
		return elem;
	}

	int AddToTail(const T& src)
	{
		// Can't insert something that's in the list... reallocation may hose us
		return InsertBefore(m_Size, src);
	}

	int Find(const T& src) const
	{
		for (int i = 0; i < Count(); ++i)
		{
			if (Element(i) == src)
			{
				return i;
			}
		}
		return -1;
	}

	void Remove(int elem)
	{
		Destruct(&Element(elem));
		ShiftElementsLeft(elem);
		--m_Size;
	}

	bool FindAndRemove(const T& src)
	{
		int elem = Find(src);
		if (elem != -1)
		{
			Remove(elem);
			return true;
		}
		return false;
	}

	iterator begin()
	{
		return Base();
	}

	const_iterator begin() const
	{
		return Base();
	}

	iterator end()
	{
		return Base() + Count();
	}

	const_iterator end() const
	{
		return Base() + Count();
	}

	//protected:
	CAllocator m_Memory;
	int m_Size;
	T* m_pElements;
};

template< typename T, class A >
T& CUtlVector< T, A >::operator[](int i)
{
	assert(i < m_Size);
	return m_Memory[i];
}

template< typename T, class A >
const T& CUtlVector< T, A >::operator[](int i) const
{
	assert(i < m_Size);
	return m_Memory[i];
}

template <class T, class A = CUtlMemory<T>>
class CCopyableUtlVector : public CUtlVector<T, A>
{
public:
	virtual ~CCopyableUtlVector();
};

template <typename I>
struct UtlRBTreeLinks_t
{
	I m_Left;
	I m_Right;
	I m_Parent;
	I m_Tag;
};

template <typename T, typename I>
struct UtlRBTreeNode_t : UtlRBTreeLinks_t<I>
{
	T m_Data;
};

template <typename T, typename I = unsigned short, typename L = bool(*)(const T&, const T&), typename M = CUtlMemory<UtlRBTreeNode_t<T, I>, I>>
class CUtlRBTree
{
public:
	T& Element(I i)
	{
		return m_Elements.Element(i).m_Data;
	}

	const T& Element(I i) const
	{
		return m_Elements.Element(i).m_Data;
	}

	I MaxElement() const
	{
		return I(m_Elements.NumAllocated());
	}

	I LeftChild(I i) const
	{
		return Links(i).m_Left;
	}

	I RightChild(I i) const
	{
		return Links(i).m_Right;
	}

	I Parent(I i) const
	{
		return Links(i).m_Parent;
	}

	bool IsRightChild(I i) const
	{
		return RightChild(Parent(i)) == i;
	}

	bool IsValidIndex(I i) const
	{
		if (i < 0)
			return false;

		if (i > m_LastAlloc.index)
			return false;

		return LeftChild(i) != i;
	}

	I Find(const T& Search) const
	{
		I Current = m_Root;
		while (Current != I(-1))
		{
			if (m_LessFunc(Search, Element(Current)))
				Current = LeftChild(Current);
			else if (m_LessFunc(Element(Current), Search))
				Current = RightChild(Current);
			else
				break;
		}

		return Current;
	}

	I FirstInorder() const
	{
		I i = m_Root;
		while (LeftChild(i) != I(-1))
			i = LeftChild(i);

		return i;
	}

	I NextInorder(I i) const
	{
		if (RightChild(i) != I(-1))
		{
			i = RightChild(i);
			while (LeftChild(i) != I(-1))
				i = LeftChild(i);

			return i;
		}

		I p = Parent(i);
		while (IsRightChild(i))
		{
			i = p;
			if (i == I(-1))
				break;

			p = Parent(i);
		}

		return p;
	}

protected:
	const UtlRBTreeLinks_t<I>& Links(I i) const
	{
		static UtlRBTreeLinks_t<I> s_Sentinel = { I(-1), I(-1), I(-1), I(1) };
		return i == I(-1) ? s_Sentinel : *reinterpret_cast<const UtlRBTreeLinks_t<I>*>(&m_Elements.Element(i));
	}

	L m_LessFunc;

	M m_Elements;
	I m_Root;
	I m_NumElements;
	I m_FirstFree;
	typename M::Iterator_t m_LastAlloc;

	UtlRBTreeNode_t<T, I>* m_pElements;
};

template <typename K, typename T, typename I = int>
class CUtlMap
{
public:
	using LessFunc_t = bool(__cdecl*)(const K&, const K&);

	T& operator[](I i)
	{
		return m_Tree.Element(i).Element;
	}

	K& Key(I i)
	{
		return m_Tree.Element(i).Key;
	}

	const K& Key(I i) const
	{
		return m_Tree.Element(i).Key;
	}

	I MaxElement() const
	{
		return m_Tree.MaxElement();
	}

	bool IsValidIndex(I i) const
	{
		return m_Tree.IsValidIndex(i);
	}

	I Find(const K& Key) const
	{
		Node_t Dummy;
		Dummy.Key = Key;
		return m_Tree.Find(Dummy);
	}

	I FirstInorder() const
	{
		return m_Tree.FirstInorder();
	}

	I NextInorder(I i) const
	{
		return m_Tree.NextInorder(i);
	}

	struct Node_t
	{
		K Key;
		T Element;
	};

	class CKeyLess
	{
	public:
		explicit CKeyLess(LessFunc_t lessFunc) : m_LessFunc(lessFunc) {}

		bool operator!() const
		{
			return !m_LessFunc;
		}

		bool operator()(const Node_t& left, const Node_t& right) const
		{
			return m_LessFunc(left.Key, right.Key);
		}

		LessFunc_t m_LessFunc;
	};

protected:
	CUtlRBTree<Node_t, I, CKeyLess> m_Tree;
};

template <typename K, typename T, typename I = int>
class CUtlHashMapLarge : public CUtlMap<K, T, I> // I'm pretty sure this doesn't inhernit from CUtlMap but for what I'm doing it's good enough
{
public:
	I Find(const K& Key) const // Temporary. Valve uses MurmurHash3.
	{
		FOR_EACH_MAP_FAST(*this, i)
		{
			if (this->Key(i) == Key)
				return i;
		}

		return -1;
	}
};

class CUtlString
{
public:
	char* Get() const
	{
		return reinterpret_cast<char*>(m_Memory.m_pMemory);
	}

	CUtlMemory<unsigned char> m_Memory;
	int m_nActualLength;
};

#define FOR_EACH_VEC( vecName, iteratorName ) \
	for ( int iteratorName = 0; iteratorName < vecName.Count(); iteratorName++ )

#define FOR_EACH_MAP( mapName, iteratorName ) \
	for ( int iteratorName = mapName.FirstInorder(); iteratorName != -1; iteratorName = mapName.NextInorder( iteratorName ) )

#define FOR_EACH_MAP_FAST( mapName, iteratorName ) \
for ( int iteratorName = 0; iteratorName < (mapName).MaxElement(); ++iteratorName ) if ( !(mapName).IsValidIndex( iteratorName ) ) continue; else
