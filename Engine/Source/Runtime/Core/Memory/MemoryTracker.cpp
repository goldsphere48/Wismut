#include "Core/Memory/MemoryTracker.h"

#include <mutex>

#include "Core/Containers/UntrackedContainers.h"
#include "Core/Math/Math.h"
#include "EngineSettigns.h"

namespace Wi
{
	struct AllocInfo
	{
		const void*	Address;
		uint64		Size;
		int			Line;
		MemoryTag	Tag;
		const char*	Filename = nullptr;
	};

	static UntrackedMap<const void*, AllocInfo> GMemoryTrackingMap;
	static UntrackedStack<MemoryTag> GMemoryTagsStack;
	static MemoryStats GStats;
	static std::mutex GMemoryTraceMutex;
	static std::mutex GUntrackedMemoryTraceMutex;

#if ENABLE_MEMORY_TRACE
	static bool GMemoryTrackerEnabled = true;
#else
	static bool GMemoryTrackerEnabled = true;
#endif

	MemoryTagScope::MemoryTagScope(MemoryTag tag)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GMemoryTraceMutex);
		GMemoryTagsStack.push(tag);
	}

	MemoryTagScope::~MemoryTagScope()
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GMemoryTraceMutex);
		GMemoryTagsStack.pop();
	}

	void MemoryTracker::Initialize()
	{
		if (!GMemoryTrackerEnabled)
			return;

		GMemoryTagsStack.push(MemoryTag::Default);
	}

	void MemoryTracker::TrackAllocation(const void* ptr, uint64 size, const char* filename, int line)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GMemoryTraceMutex);
		MemoryTag tag = GMemoryTagsStack.top();
		AllocInfo info;
		info.Size = size;
		info.Address = ptr;
		info.Tag = tag;
		info.Line = line;
		info.Filename = filename;

		GMemoryTrackingMap[ptr] = info;

		GStats.TotalAllocated += size;
		GStats.CurrentUsed += size;
		GStats.PeakUsed = Math::Max(GStats.PeakUsed, GStats.CurrentUsed);
		GStats.TotalAllocations++;

		MemoryTagStats& tagStats = GStats.TagStats[static_cast<int>(tag)];
		tagStats.TotalAllocated += size;
		tagStats.CurrentUsed += size;
		tagStats.PeakUsed = Math::Max(GStats.PeakUsed, GStats.CurrentUsed);
		tagStats.TotalAllocations++;
	}

	void MemoryTracker::TrackFree(const void* ptr)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GMemoryTraceMutex);

		auto it = GMemoryTrackingMap.find(ptr);
		if (it != GMemoryTrackingMap.end()) {
			AllocInfo info = it->second;
			GMemoryTrackingMap.erase(it);

			GStats.CurrentUsed -= info.Size;
			GStats.TotalFrees++;

			MemoryTagStats& tagStats = GStats.TagStats[static_cast<int>(info.Tag)];
			tagStats.CurrentUsed -= info.Size;
			tagStats.TotalFrees++;
		}
	}

	void MemoryTracker::TrackUntrackedMemoryAllocation(uint64 size)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GUntrackedMemoryTraceMutex);

		GStats.UntrackedMemoryAllocated += size;
	}

	void MemoryTracker::TrackUntrackedMemoryFree(uint64 size)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GUntrackedMemoryTraceMutex);

		GStats.UntrackedMemoryAllocated -= size;
	}

	const MemoryStats& MemoryTracker::GetStats()
	{
		return GStats;
	}

	bool MemoryTracker::IsEnabled()
	{
		return GMemoryTrackerEnabled;
	}
}
