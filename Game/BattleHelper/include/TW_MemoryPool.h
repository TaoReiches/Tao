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
class TeMemoryPool
{
private:
	union MemoryPoolElement
	{
	private:
		using StorageType = char[sizeof(T)];

		MemoryPoolElement* next;
		StorageType datum alignas(alignof(T));

	public:
		MemoryPoolElement* GetNextItem() const { return next; }
		void SetNextItem(MemoryPoolElement* e) { next = e; }

		T* GetStorage() { return reinterpret_cast<T*>(datum); }

		static MemoryPoolElement* StorageToItem(T* t)
		{
			MemoryPoolElement* current = reinterpret_cast<MemoryPoolElement*>(t);
			return current;
		}
	};

	class MemoryPoolStore
	{
	private:
		std::unique_ptr<MemoryPoolElement[]> storage;
		std::unique_ptr<MemoryPoolStore> next;

	public:
		MemoryPoolStore(size_t size) : storage(new MemoryPoolElement[size])
		{
			for (size_t i = 1; i < size; ++i)
			{
				storage[i - 1].SetNextItem(&storage[i]);
			}
			storage[size - 1].SetNextItem(nullptr);
		}

		MemoryPoolElement* GetStorage() const { return storage.get(); }

		void SetNextStore(std::unique_ptr<MemoryPoolStore>&& n)
		{
			assert(!next);

			next.reset(n.release());
		}
	};

	size_t storeSize;
	std::unique_ptr<MemoryPoolStore> store;
	MemoryPoolElement* freeList;

public:
	MemoryPool(size_t size) : storeSize(size), store(new MemoryPoolStore(size))
	{
		freeList = store->GetStorage();
	}

	template <typename... Args>
	T* alloc(Args&&... args)
	{
		if (freeList == nullptr)
		{
			std::unique_ptr<MemoryPoolStore> newStore(new MemoryPoolStore(storeSize));

			newStore->SetNextStore(std::move(store));
			store.reset(newStore.release());
			freeList = store->GetStorage();
		}

		MemoryPoolElement* current = freeList;
		freeList = current->GetNextItem();

		T* result = current->GetStorage();
		new (result) T(std::forward<Args>(args)...);

		return result;
	}

	void free(T* t)
	{
		t->T::~T();

		MemoryPoolElement* current = MemoryPoolElement::StorageToItem(t);

		current->SetNextItem(freeList);
		freeList = current;
	}
};
