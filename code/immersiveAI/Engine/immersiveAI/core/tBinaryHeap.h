//-------------------------------------------------------------------
// Immersive AI :: Core :: tBinaryHeap
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file tBinaryHeap.h
//-------------------------------------------------------------------
/// @class BinaryHeap
/// @author Gavin Bunney
/// @version 1.0
/// @brief Binary Heap template object.
/// 
/// Implementation of a binary heap (template) object.
//-------------------------------------------------------------------
#ifndef _TBINARYHEAP_H_
#define _TBINARYHEAP_H_

#ifndef _PLATFORM_H_
	#include "platform/platform.h"
#endif

//-------------------------------------------------------------------
/// @def BINARYHEAP_COMPARE
/// @brief Compare function callback type for comparing elements in
///        the binary heap.
//-------------------------------------------------------------------
#define BINARYHEAP_COMPARE FN_CDECL

template<class T> class BinaryHeap
{

public:

	//-------------------------------------------------------------------
	/// @fn BinaryHeap(const U32 heapSize, S32 (BINARYHEAP_COMPARE *fcmp)
	///                                    (const void *, const void *)) 
	/// @brief Creates a new BinaryHeap with the parsed size and using
	///        the parsed compare function.
	///
	/// @param heapSize Size of the binary heap.
	//-------------------------------------------------------------------
	BinaryHeap(const U32 heapSize, S32 (BINARYHEAP_COMPARE *fcmp)(const void *, const void *));

	//-------------------------------------------------------------------
	/// @fn ~BinaryHeap() 
	/// @brief Deconstructor which frees the binary heaps memory.
	//-------------------------------------------------------------------
	~BinaryHeap();

	//-------------------------------------------------------------------
	/// @fn S32 size() const
	/// @brief Returns the number of elements contained within the heap.
	///
	/// @return S32 Number of elements in the heap.
	//-------------------------------------------------------------------
	S32 size() const;

	//-------------------------------------------------------------------
	/// @fn bool empty() const
	/// @brief True if the heap contains no elements; False otherwise.
	///
	/// @return Heap contains no elements.
	//-------------------------------------------------------------------
	bool empty() const;

	//-------------------------------------------------------------------
	/// @fn T& front()
	/// @brief Retrieves the front element of the binary heap.
	///
	/// @return T& Front element.
	//-------------------------------------------------------------------
	T& front();

	//-------------------------------------------------------------------
	/// @fn const T& front() const
	/// @brief Retrieves the front element of the binary heap, enforcing
	///        const.
	///
	/// @return T& Front element.
	//-------------------------------------------------------------------
	const T& front() const;

	//-------------------------------------------------------------------
	/// @fn void push(const T& x)
	/// @brief Adds an element to the binary heap.
	///
	/// @param x Element to add to the heap.
	//-------------------------------------------------------------------
	void push(const T& x);

	//-------------------------------------------------------------------
	/// @fn void pop()
	/// @brief Removes the front element from the binary heap.
	//-------------------------------------------------------------------
	void pop();

	//-------------------------------------------------------------------
	/// @fn T& operator[](U32 index)
	/// @brief Index operator to retrieve the element at the specified
	///        U32 index (array indexed from 1).
	///
	/// @param index of the element (array indexed from 1).
	/// @return T& Element at specifed index.
	//-------------------------------------------------------------------
	T& operator[](U32 index);

	//-------------------------------------------------------------------
	/// @fn const T& operator[](U32 index) const;
	/// @brief Index operator to retrieve the element at the specified
	///        U32 index (array indexed from 1), enforcing const.
	///
	/// @param index of the element (array indexed from 1).
	/// @return T& Element at specifed index.
	//-------------------------------------------------------------------
	const T& operator[](U32 index) const;

	//-------------------------------------------------------------------
	/// @fn const T& operator[](S32 i) const;
	/// @brief Index operator to retrieve the element at the specified
	///        S32 index (array indexed from 1), enforcing const.
	///
	/// @param i Index of the element (array indexed from 1).
	/// @return T& Element at specifed index.
	//-------------------------------------------------------------------
	T& operator[](S32 i) { return operator[](U32(i)); }

	//-------------------------------------------------------------------
	/// @fn const T& operator[](S32 i) const;
	/// @brief Index operator to retrieve the element at the specified
	///        S32 index, (array indexed from 1) enforcing const.
	///
	/// @param i Index of the element (array indexed from 1).
	/// @return T& Element at specifed index.
	//-------------------------------------------------------------------
	const T& operator[](S32 i) const { return operator[](U32(i)); }

	//-------------------------------------------------------------------
	/// @fn const T& operator[](S32 i) const;
	/// @brief Index operator to retrieve the element at the specified
	///        S32 index, (array indexed from 1) enforcing const.
	///
	/// @param i Index of the element (array indexed from 1).
	/// @return U32 Max number of elements placeable in this heap.
	//-------------------------------------------------------------------
	U32 capacity() const;

	//-------------------------------------------------------------------
	/// @fn T* address() const;
	/// @brief Address of the binary heap.
	///
	/// @return T* Memory address of the binary heap array.
	//-------------------------------------------------------------------
	T* address() const;

protected:

	//-------------------------------------------------------------------
	/// @var U32 mElementCount
	/// @brief Count of elements contained in the binary heap.
	//-------------------------------------------------------------------
	U32 mElementCount;
	
	//-------------------------------------------------------------------
	/// @var U32 mArraySize
	/// @brief Size of the binary heap array.
	//-------------------------------------------------------------------
	U32 mArraySize;
	
	//-------------------------------------------------------------------
	/// @var T* mArray
	/// @brief Array of data contained in the binary heap.
	//-------------------------------------------------------------------
	T* mArray;
	
	//-------------------------------------------------------------------
	/// @var S32 (BINARYHEAP_COMPARE *mCompareFunction)(const void *,
	///                                                 const void *)
	/// @brief Pointer to the compare function used when sorting the
	///        binary heap.
	///
	/// When the function is executed, a result of -1, 0 or 1 is
	/// expected.<br>
	/// Where:<br>
	/// -1: element a &lt; element b<br>
	/// 0: element a == element b<br>
	/// 1: element a &gt; element b<br>
	//-------------------------------------------------------------------
	S32 (BINARYHEAP_COMPARE *mCompareFunction)(const void *, const void *);
};

template<class T> inline BinaryHeap<T>::~BinaryHeap()
{
	this->mCompareFunction = 0;
	delete[] this->mArray;
}

template<class T> inline BinaryHeap<T>::BinaryHeap(const U32 heapSize, S32 (BINARYHEAP_COMPARE *fcmp)(const void *, const void *))
{
	// array size is +1 as heap is indexed from 1
	this->mArraySize = heapSize + 1;
	this->mArray = new T[this->mArraySize];
	this->mElementCount = 0;
	this->mCompareFunction = fcmp;
}

template<class T> inline T* BinaryHeap<T>::address() const
{
	return this->mArray;
}

template<class T> inline T& BinaryHeap<T>::front()
{
	return this->mArray[1];
}

template<class T> inline const T& BinaryHeap<T>::front() const
{
	return this->mArray[1];
}

template<class T> inline S32 BinaryHeap<T>::size() const
{
	return (S32)this->mElementCount;
}

template<class T> inline bool BinaryHeap<T>::empty() const
{
	return (this->mElementCount == 0);
}

template<class T> inline void BinaryHeap<T>::push(const T& x)
{
	++this->mElementCount;
	this->mArray[this->mElementCount] = x;

	// reorder the openList
	U32 iter = this->mElementCount;
	while (iter != 1)
	{
		// check if child <= parent
		if (this->mCompareFunction(this->mArray[iter], this->mArray[iter/2]) < 1)
		{
			T temp = this->mArray[iter/2];
			this->mArray[iter/2] = this->mArray[iter];
			this->mArray[iter] = temp;
			iter = iter / 2;
		} else
		{
			break;
		}
	}
}

template<class T> inline void BinaryHeap<T>::pop()
{
	// remove the first element
	this->mArray[1] = this->mArray[this->mElementCount];
	--this->mElementCount;

	U32 u = 0, v = 1;
	while (u != v) 
	{
		u = v;
		
		// if both children exist
		if ((2*u + 1) <= this->mElementCount)
		{
			// check if parent is greater than each child
			if (this->mCompareFunction(this->mArray[v], this->mArray[2*u]) > -1)
				v = 2*u; // child 1
			if (this->mCompareFunction(this->mArray[v], this->mArray[2*u+1]) > -1)
				v = 2*u+1; // child 2
		} else
		{
			// check if only 1 child
			if (2*u <= this->mElementCount) 
			{
				// check if parent is greater than child 1
				if (this->mCompareFunction(this->mArray[u], this->mArray[2*u]) > -1)
					v = 2*u;
			}
		}

		// if parent is > one of its children, swap them
		if (u != v)
		{
			T temp = this->mArray[u];
			this->mArray[u] = this->mArray[v];
			this->mArray[v] = temp;		
		}
	}
}

template<class T> inline T& BinaryHeap<T>::operator[](U32 index)
{
	return this->mArray[index+1];
}

template<class T> inline const T& BinaryHeap<T>::operator[](U32 index) const
{
	return this->mArray[index+1];
}

template<class T> inline U32 BinaryHeap<T>::capacity() const
{
    return this->mArraySize;
}

#endif