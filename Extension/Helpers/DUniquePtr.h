#pragma once

template<typename T>
class DUniquePtr
{
public:
	DUniquePtr(T* Ptr)
	{
		element = Ptr;
	}
	DUniquePtr()
	{
		element = nullptr;
	}

	~DUniquePtr()
	{
		Clean();
	}

	DUniquePtr(DUniquePtr& obj)
	{
		element = obj.element;

		//Invalidate the other
		obj.element = nullptr;
	}

	DUniquePtr& operator=(const DUniquePtr& obj) = delete;

	void Move(DUniquePtr<T>& t)
	{
		Clean();
		this->element = t.element;
		t.element = nullptr;
	}
	void Set(T* ptr)
	{
		Clean();
		this->element = ptr;
	}
	T* Get()const { return element; }

	bool IsValid()const { return element != nullptr; }

	T* operator->() const { return element; }

	operator T* () const { return element; }

	explicit operator bool() const { return element; }

private:
	void Clean()
	{
		if (element != nullptr)
		{
			delete element;
			element = nullptr;
		}
	}

	T* element = nullptr;
};