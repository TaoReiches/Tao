#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>

template <typename T>
class TwMemoryPool
{
private:
	union TeMemoryPoolElement
	{
	private:
		using StorageType = char[sizeof(T)];

		TeMemoryPoolElement* next;
		StorageType datum alignas(alignof(T));

	public:
		TeMemoryPoolElement* GetNextItem() const { return next; }
		void SetNextItem(TeMemoryPoolElement* e) { next = e; }

		T* GetStorage() { return reinterpret_cast<T*>(datum); }

		static TeMemoryPoolElement* StorageToItem(T* t)
		{
			TeMemoryPoolElement* current = reinterpret_cast<TeMemoryPoolElement*>(t);
			return current;
		}
	};

	class TeMemoryPoolStore
	{
	private:
		std::unique_ptr<TeMemoryPoolElement[]> storage;
		std::unique_ptr<TeMemoryPoolStore> next;

	public:
		TeMemoryPoolStore(size_t size) : storage(new TeMemoryPoolElement[size])
		{
			for (size_t i = 1; i < size; ++i)
			{
				storage[i - 1].SetNextItem(&storage[i]);
			}
			storage[size - 1].SetNextItem(nullptr);
		}

		TeMemoryPoolElement* GetStorage() const { return storage.get(); }

		void SetNextStore(std::unique_ptr<TeMemoryPoolStore>&& n)
		{
			assert(!next);

			next.reset(n.release());
		}
	};

	size_t storeSize;
	std::unique_ptr<TeMemoryPoolStore> store;
	TeMemoryPoolElement* freeList;

public:
	TwMemoryPool(size_t size) : storeSize(size), store(new TeMemoryPoolStore(size))
	{
		freeList = store->GetStorage();
	}

	template <typename... Args>
	T* alloc(Args&&... args)
	{
		if (freeList == nullptr)
		{
			std::unique_ptr<TeMemoryPoolStore> newStore(new TeMemoryPoolStore(storeSize));

			newStore->SetNextStore(std::move(store));
			store.reset(newStore.release());
			freeList = store->GetStorage();
		}

		TeMemoryPoolElement* current = freeList;
		freeList = current->GetNextItem();

		T* result = current->GetStorage();
		new (result) T(std::forward<Args>(args)...);

		return result;
	}

	void free(T* t)
	{
		t->T::~T();

		TeMemoryPoolElement* current = TeMemoryPoolElement::StorageToItem(t);

		current->SetNextItem(freeList);
		freeList = current;
	}
};
