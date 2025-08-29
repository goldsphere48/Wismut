#pragma once

#include "MemoryBase.h"
#include <mutex>

namespace Wi
{
	class SystemMalloc final : public IMalloc
	{
	public:
		virtual void* Allocate(usize size, usize alignment = 0) override;
		virtual void Free(void* ptr) override;

	private:
		std::mutex m_Mutex;
	};
}
