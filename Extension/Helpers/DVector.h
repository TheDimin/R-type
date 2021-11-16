#pragma once
#include <cassert>
#include <cstring>

#include "DUniquePtr.h"

template<typename T>
class DVectorBase
{
public:
	DVectorBase()
	{
		elements = new T[1];
	}

	DVectorBase(int size, bool AllActive = false)
	{
		elements = new T[size];
		maxElements = size;
		if (AllActive)
			activeElements = size;
	}

	DVectorBase(const DVectorBase& Other) = delete;

	DVectorBase(DVectorBase&& Other) = delete;

	DVectorBase<T>& operator=(const DVectorBase<T>& Other)
	{
		if (this == &Other)
			return *this;

		maxElements = Other.maxElements;
		activeElements = Other.activeElements;

		delete[] elements;

		elements = new T[maxElements];
		memcpy(elements, Other.elements, sizeof(T) * activeElements);

		return *this;
	}

	T& operator [] (int i) const
	{
		if (i > activeElements)
			return elements[0];
		assert(i <= activeElements);
		return elements[i];
	}


	//TODO supporting Remove requires Elemnt shifting
	int Add(const T& NewElement);

	void RemoveIndex(const int index)
	{
		memmove(elements + index, elements + index + 1, sizeof(T) * (activeElements - index));

		activeElements--;
		//assert(false && "failed to find element to remove");
	}


	void Remove(const T ToRemove)
	{
		for (int i = 0; i < activeElements; ++i)
		{
			if (ToRemove == elements[i]) {

				memmove(elements + i, elements + i + 1, sizeof(T) * (activeElements - i));

				activeElements--;
				//We found the element delete
				//TODO add version that deletes all found results ?
				return;
			}
		}
	}

	bool Contains(bool (*it)(T&))
	{
		for (int i = 0; i < maxElements; ++i)
		{
			if (elements[i] == nullptr) continue;

			if (it(elements[i]))
				return true;
		}
		return false;
	}

	void Resize(int newSize, bool AllActive)
	{
		Resize(newSize);
		if (AllActive)
			activeElements = newSize - 1;
	}

	void Resize(int newSize)
	{
		if (newSize == maxElements)return;
		assert(maxElements < newSize);
		T* newArray = new T[newSize];

		memcpy(newArray, elements, sizeof(T) * activeElements);
		memset(elements, 0, sizeof(T) * activeElements);

		delete[] elements;

		elements = newArray;

		maxElements = newSize;
	}

	void Clear()
	{
		activeElements = 0;
	}

	int Size()const { return activeElements; }
	int Storage() const { return maxElements; }

public:
	~DVectorBase()
	{
		delete[] elements;
	}

protected:
	T* elements = nullptr;
	int maxElements = 1;
	int activeElements = 0;
};


template<typename  T>
class DVector : public DVectorBase<T>
{
public:
	DVector() : DVectorBase() {}

	DVector(int size, bool AllActive = false) :DVectorBase(size, AllActive) {}

};

template<typename  T>
class DVector<DUniquePtr<T>> : public DVectorBase<DUniquePtr<T>>
{
public:
	DVector() : DVectorBase() {}

	DVector(int size, bool AllActive = false) :DVectorBase(size, AllActive) {};

	T* operator [] (const int i)const { return elements[i]; }
	bool Contains(T* toFind) const
	{
		for (int i = 0; i < maxElements; ++i)
		{
			if (!elements[i].IsValid()) continue;

			if (elements[i] == toFind)
				return true;
		}
		return false;
	}

	int Add(const T& NewElement) = delete;
	int Add(T* NewElement);
	int Add(const DUniquePtr<T>& NewElement);
};

template <typename T>
int DVector<DUniquePtr<T>>::Add(T* NewElement)
{
	if (maxElements == activeElements)
	{
		Resize(maxElements * 2);
	}

	elements[activeElements].Set(NewElement);
	activeElements++;

	return activeElements - 1;
}

template <typename T>
int DVector<DUniquePtr<T>>::Add(const DUniquePtr<T>& NewElement)
{

	if (maxElements == activeElements)
	{
		Resize(maxElements + 10);
	}

	elements[activeElements].Move(NewElement);

	activeElements++;

	return activeElements - 1;
}

template <typename T>
int DVectorBase<T>::Add(const T& NewElement)
{

	if (maxElements == activeElements)
	{
		Resize(maxElements + 10);
	}

	elements[activeElements] = NewElement;

	activeElements++;

	return activeElements - 1;
}