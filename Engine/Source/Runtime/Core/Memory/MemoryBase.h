#pragma once

#include "Core/CommonDefines.h"
#include "Core/CoreTypes.h"

namespace Wi
{
	class  UseSystemMalloc
	{
	public:
		void* operator new(std::size_t size);
		void* operator new[](std::size_t size);
		void  operator delete(void* p) noexcept;
		void  operator delete[](void* p) noexcept;
	};

	class IMalloc : public UseSystemMalloc
	{
	public:
		DELETE_COPYING(IMalloc);
		virtual ~IMalloc() = default;

		virtual void* Allocate(usize size, usize alignment = 0) = 0;
		virtual void Free(void* ptr) = 0;
	};
}
