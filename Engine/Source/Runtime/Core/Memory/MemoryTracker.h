#pragma once

#include "Core/CoreTypes.h"

namespace Wi
{
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
		uint64 TotalAllocated;
		uint64 CurrentUsed;
		uint64 PeakUsed;
		uint64 TotalAllocations;
		uint64 TotalFrees;
	};

	struct MemoryStats
	{
		uint64 TotalAllocated;
		uint64 PeakUsed;
		uint64 CurrentUsed;
		uint64 TotalAllocations;
		uint64 TotalFrees;
		uint64 UntrackedMemoryAllocated;
		MemoryTagStats TagStats[static_cast<uint8>(MemoryTag::Count)];
	};

	class Logger;

	class MemoryTracker
	{
	public:
		static void Initialize();

		static void TrackAllocation(const void* ptr, uint64 size, const char* filename = nullptr, int line = 0);
		static void TrackFree(const void* ptr);

		static void TrackUntrackedMemoryAllocation(uint64 size);
		static void TrackUntrackedMemoryFree(uint64 size);

		static void DumpMemoryStats(const Logger& logger);
		static void DumpMemoryLeaks(const Logger& logger);

		static const MemoryStats& GetStats();
		static bool IsEnabled();
	};
}
