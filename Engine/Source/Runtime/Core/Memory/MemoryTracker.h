#pragma once

#include <unordered_map>

#include "Core/CoreTypes.h"

namespace Wi
{
	struct AllocInfo;
#define MEMORY_TAG_SCOPE(x) Wi::MemoryTagScope Scope_##__LINE__(x);

	enum class MemoryTag : uint8
	{
		Default,
		Rendering,
		Count,
	};

	struct MemoryTagScope
	{
		MemoryTagScope(MemoryTag tag);
		~MemoryTagScope();
	};

	struct MemoryTagStats
	{
		uint64 TotalAllocated = 0;
		uint64 CurrentUsed = 0;
		uint64 PeakUsed = 0;
		uint64 TotalAllocations = 0;
		uint64 TotalFrees = 0;
	};

	struct MemoryStats
	{
		uint64 TotalAllocated = 0;
		uint64 PeakUsed = 0;
		uint64 CurrentUsed = 0;
		uint64 TotalAllocations = 0;
		uint64 TotalFrees = 0;
		uint64 UntrackedMemoryAllocated = 0;
		MemoryTagStats TagStats[static_cast<uint8>(MemoryTag::Count)];
	};

	class Logger;

	class MemoryTracker
	{
	private:
		struct AllocInfo
		{
			const void* Address;
			uint64		Size;
			int			Line;
			MemoryTag	Tag;
			const char* Filename = nullptr;

			bool operator<(const AllocInfo& other) const
			{
				return Size < other.Size;
			}
		};

	public:
		static MemoryTracker* GetInstance()
		{
			static MemoryTracker instance;
			return &instance;
		}

		void TrackAllocation(const void* ptr, uint64 size, const char* filename = nullptr, int line = 0);
		void TrackReallocation(void* oldPtr, const void* newPtr, uint64 newSize, const char* filename = nullptr, int line = 0);
		void TrackFree(const void* ptr);

		void DumpMemoryStats(const Logger& logger);
		void DumpMemoryLeaks(const Logger& logger);

		const MemoryStats& GetStats() const;
		static bool IsEnabled();

		void ClearStats();

	private:
		MemoryStats m_Stats;
		std::unordered_map<const void*, AllocInfo> m_ActiveAllocations;
	};
}
