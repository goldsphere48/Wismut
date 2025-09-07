#include "Core/Memory/MemoryTracker.h"

#include <mutex>
#include <ranges>

#include "Core/Containers/UntrackedContainers.h"
#include "Core/Math/Math.h"
#include "EngineSettigns.h"
#include "Core/Logger/Logger.h"
#include "Core/Utils/EnumUtils.h"

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

	static UntrackedMap<const void*, AllocInfo>& GetActiveAllocations()
	{
		static UntrackedMap<const void*, AllocInfo> instance;
		return instance;
	}

	static UntrackedStack<MemoryTag>& GetMemoryTagsStack()
	{
		static UntrackedStack<MemoryTag> instance;
		return instance;
	}

	static MemoryStats& GetStatsInternal()
	{
		static MemoryStats instance;
		return instance;
	}

	static std::mutex& GetMemoryTraceMutex()
	{
		static std::mutex instance;
		return instance;
	}

	static std::mutex& GetUntrackedMemoryTraceMutex()
	{
		static std::mutex instance;
		return instance;
	}

#if ENABLE_MEMORY_TRACE
	static bool GMemoryTrackerEnabled = true;
#else
	static bool GMemoryTrackerEnabled = true;
#endif

	MemoryTagScope::MemoryTagScope(MemoryTag tag)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GetMemoryTraceMutex());
		GetMemoryTagsStack().push(tag);
	}

	MemoryTagScope::~MemoryTagScope()
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GetMemoryTraceMutex());
		GetMemoryTagsStack().pop();
	}

	void MemoryTracker::TrackAllocation(const void* ptr, uint64 size, const char* filename, int line)
	{
		if (!GMemoryTrackerEnabled)
			return;

		auto tagStack = GetMemoryTagsStack();
		std::lock_guard lock(GetMemoryTraceMutex());
		MemoryTag tag = tagStack.empty() ? MemoryTag::Default : tagStack.top();
		AllocInfo info;
		info.Size = size;
		info.Address = ptr;
		info.Tag = tag;
		info.Line = line;
		info.Filename = filename;

		GetActiveAllocations()[ptr] = info;

		MemoryStats& stats = GetStatsInternal();
		stats.TotalAllocated += size;
		stats.CurrentUsed += size;
		stats.PeakUsed = Math::Max(stats.PeakUsed, stats.CurrentUsed);
		stats.TotalAllocations++;

		MemoryTagStats& tagStats = stats.TagStats[static_cast<int>(tag)];
		tagStats.TotalAllocated += size;
		tagStats.CurrentUsed += size;
		tagStats.PeakUsed = Math::Max(stats.PeakUsed, stats.CurrentUsed);
		tagStats.TotalAllocations++;
	}

	void MemoryTracker::TrackFree(const void* ptr)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GetMemoryTraceMutex());
		auto& activeAllocations = GetActiveAllocations();

		auto it = activeAllocations.find(ptr);
		if (it != activeAllocations.end()) {
			AllocInfo info = it->second;
			activeAllocations.erase(it);

			MemoryStats& stats = GetStatsInternal();

			stats.CurrentUsed -= info.Size;
			stats.TotalFrees++;

			MemoryTagStats& tagStats = stats.TagStats[static_cast<int>(info.Tag)];
			tagStats.CurrentUsed -= info.Size;
			tagStats.TotalFrees++;
		}
	}

	void MemoryTracker::TrackUntrackedMemoryAllocation(uint64 size)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GetUntrackedMemoryTraceMutex());

		GetStatsInternal().UntrackedMemoryAllocated += size;
	}

	void MemoryTracker::TrackUntrackedMemoryFree(uint64 size)
	{
		if (!GMemoryTrackerEnabled)
			return;

		std::lock_guard lock(GetUntrackedMemoryTraceMutex());

		GetStatsInternal().UntrackedMemoryAllocated -= size;
	}

	void MemoryTracker::DumpMemoryStats(const Logger& logger)
	{
		MemoryStats& stats = GetStatsInternal();
		logger.Log(LogLevel::Info, "================== Memory Statistic ==================");
		logger.Log(LogLevel::Info, "Total allocated memory:  {0}", stats.TotalAllocated);
		logger.Log(LogLevel::Info, "Current used memory:     {0}", stats.CurrentUsed);
		logger.Log(LogLevel::Info, "Peak used memory:        {0}", stats.PeakUsed);
		logger.Log(LogLevel::Info, "Untracked memory used:   {0}", stats.UntrackedMemoryAllocated);
		logger.Log(LogLevel::Info, "Total allocations count: {0}", stats.TotalAllocations);
		logger.Log(LogLevel::Info, "Total frees count:       {0}", stats.TotalFrees);

		for (int i = 0; i < static_cast<int>(MemoryTag::Count); ++i)
		{
			MemoryTag tag = static_cast<MemoryTag>(i);
			MemoryTagStats tagStats= stats.TagStats[i];

			logger.Log(LogLevel::Info, "Memory category:         {0}", Utils::EnumToString<MemoryTag>(tag));
			logger.Log(LogLevel::Info, "Total allocated memory:  {0}", tagStats.TotalAllocated);
			logger.Log(LogLevel::Info, "Current used memory:     {0}", tagStats.CurrentUsed);
			logger.Log(LogLevel::Info, "Peak used memory:        {0}", tagStats.PeakUsed);
			logger.Log(LogLevel::Info, "Total allocations count: {0}", tagStats.TotalAllocations);
			logger.Log(LogLevel::Info, "Total frees count:       {0}", tagStats.TotalFrees);
		}

		logger.Log(LogLevel::Info, "=====================================================");
	}

	void MemoryTracker::DumpMemoryLeaks(const Logger& logger)
	{
		if (!GMemoryTrackerEnabled)
			return;

		bool hasLeaks = false;
		uint64 totalLeakedBytes = 0;
		usize totalLeakedAllocations = 0;

		usize leaksByTag[static_cast<size_t>(MemoryTag::Count)] = { 0 };
		uint64 leakedBytesByTag[static_cast<size_t>(MemoryTag::Count)] = { 0 };

		usize untrackedMemory = GetStatsInternal().UntrackedMemoryAllocated;

		UntrackedVector<AllocInfo> leaksVector;

		logger.Log(LogLevel::Info, "================== Memory Leaks Report ==================");

		const auto& activeAllocations = GetActiveAllocations();
		for (const auto& allocInfo : activeAllocations | std::views::values)
		{
			hasLeaks = true;
			totalLeakedBytes += allocInfo.Size;
			totalLeakedAllocations++;

			leaksVector.push_back(allocInfo);

			size_t tagIndex = static_cast<size_t>(allocInfo.Tag);
			if (tagIndex < static_cast<size_t>(MemoryTag::Count)) {
				leaksByTag[tagIndex]++;
				leakedBytesByTag[tagIndex] += allocInfo.Size;
			}

			logger.Log(LogLevel::Warning,
				"LEAK: Address={0:X}, Size={1} bytes, Tag={2}, File={3}, Line={4}",
				reinterpret_cast<uintptr_t>(allocInfo.Address),
				allocInfo.Size,
				Utils::EnumToString<MemoryTag>(allocInfo.Tag),
				allocInfo.Filename ? allocInfo.Filename : "Unknown",
				allocInfo.Line
			);
		}

		if (hasLeaks)
		{
			logger.Log(LogLevel::Error, "=================== LEAK SUMMARY ===================");
			logger.Log(LogLevel::Error, "Total leaked allocations: {0}", totalLeakedAllocations);
			logger.Log(LogLevel::Error, "Total leaked bytes:       {0}", totalLeakedBytes);

			logger.Log(LogLevel::Error, "Leaks by category:");
			for (int i = 0; i < static_cast<int>(MemoryTag::Count); ++i)
			{
				if (leaksByTag[i] > 0)
				{
					MemoryTag tag = static_cast<MemoryTag>(i);
					logger.Log(LogLevel::Error,
						"  {0}: {1} allocations, {2} bytes",
						Utils::EnumToString<MemoryTag>(tag),
						leaksByTag[i],
						leakedBytesByTag[i]
					);
				}
			}

			for (size_t i = 0; i < leaksVector.size() - 1; ++i)
			{
				for (size_t j = 0; j < leaksVector.size() - 1 - i; ++j)
				{
					if (leaksVector[j].Size < leaksVector[j + 1].Size)
					{
						AllocInfo temp = leaksVector[j];
						leaksVector[j] = leaksVector[j + 1];
						leaksVector[j + 1] = temp;
					}
				}
			}

			if (!leaksVector.empty())
			{
				logger.Log(LogLevel::Error, "Top 10 largest leaks:");
				size_t topCount = leaksVector.size() < 10 ? leaksVector.size() : 10;
				for (size_t i = 0; i < topCount; ++i)
				{
					const auto& leak = leaksVector[i];
					logger.Log(LogLevel::Error,
						"  #{0}: {1} bytes at {2:X} ({3}:{4})",
						i + 1,
						leak.Size,
						reinterpret_cast<uintptr_t>(leak.Address),
						leak.Filename ? leak.Filename : "Unknown",
						leak.Line
					);
				}
			}
		}
		else
		{
			logger.Log(LogLevel::Info, "No memory leaks detected!");
		}

		logger.Log(LogLevel::Info, "=====================================================");
		logger.Log(LogLevel::Info, "Untracked memory at end: {0} bytes", untrackedMemory);
	}

	const MemoryStats& MemoryTracker::GetStats()
	{
		return GetStatsInternal();
	}

	bool MemoryTracker::IsEnabled()
	{
		return GMemoryTrackerEnabled;
	}

	void MemoryTracker::ClearStats()
	{
		if (!IsEnabled())
			return;

		std::lock_guard lock(GetMemoryTraceMutex());

		MemoryStats& stats = GetStatsInternal();

		stats.TotalAllocated = 0;
		stats.CurrentUsed = 0;
		stats.PeakUsed = 0;
		stats.TotalAllocations = 0;
		stats.TotalFrees = 0;

		for (int i = 0; i < static_cast<int>(MemoryTag::Count); ++i)
		{
			stats.TagStats[i] = {};
		}
	}
}
