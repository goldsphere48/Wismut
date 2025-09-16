// TODO: Сделать все потокобезопасным

#include "Core/Memory/MemoryTracker.h"

#include <ranges>
#include <stack>

#include "Core/Math/Math.h"
#include "EngineSettigns.h"
#include "Core/Logger/Logger.h"
#include "Core/Utils/EnumUtils.h"

namespace Wi
{
#if ENABLE_MEMORY_TRACE
	static bool GMemoryTrackerEnabled = true;
#else
	static bool GMemoryTrackerEnabled = false;
#endif

	static std::stack<MemoryTag>& GetMemoryTagsStack()
	{
		static std::stack<MemoryTag> instance;
		return instance;
	}

	MemoryTagScope::MemoryTagScope(MemoryTag tag)
	{
		if (!GMemoryTrackerEnabled)
			return;

		GetMemoryTagsStack().push(tag);
	}

	MemoryTagScope::~MemoryTagScope()
	{
		if (!GMemoryTrackerEnabled)
			return;

		GetMemoryTagsStack().pop();
	}

	void MemoryTracker::TrackAllocation(const void* ptr, uint64 size, const char* filename, int line)
	{
		if (!GMemoryTrackerEnabled)
			return;

		auto tagStack = GetMemoryTagsStack();
		MemoryTag tag = tagStack.empty() ? MemoryTag::Default : tagStack.top();
		AllocInfo info;
		info.Size = size;
		info.Address = ptr;
		info.Tag = tag;
		info.Line = line;
		info.Filename = filename;

		m_ActiveAllocations[ptr] = info;

		m_Stats.TotalAllocated += size;
		m_Stats.CurrentUsed += size;
		m_Stats.PeakUsed = Math::Max(m_Stats.PeakUsed, m_Stats.CurrentUsed);
		m_Stats.TotalAllocations++;

		MemoryTagStats& tagStats = m_Stats.TagStats[static_cast<int>(tag)];
		tagStats.TotalAllocated += size;
		tagStats.CurrentUsed += size;
		tagStats.PeakUsed = Math::Max(tagStats.PeakUsed, m_Stats.CurrentUsed);
		tagStats.TotalAllocations++;
	}

	void MemoryTracker::TrackFree(const void* ptr)
	{
		if (!GMemoryTrackerEnabled)
			return;

		auto it = m_ActiveAllocations.find(ptr);
		if (it != m_ActiveAllocations.end()) {
			AllocInfo info = it->second;
			m_ActiveAllocations.erase(it);

			m_Stats.CurrentUsed -= info.Size;
			m_Stats.TotalFrees++;

			MemoryTagStats& tagStats = m_Stats.TagStats[static_cast<int>(info.Tag)];
			tagStats.CurrentUsed -= info.Size;
			tagStats.TotalFrees++;
		}
	}

	void MemoryTracker::DumpMemoryStats(const Logger& logger)
	{
		logger.Log(LogLevel::Info, "================== Memory Statistic ==================");
		logger.Log(LogLevel::Info, "Total allocated memory:  {0}", m_Stats.TotalAllocated);
		logger.Log(LogLevel::Info, "Current used memory:     {0}", m_Stats.CurrentUsed);
		logger.Log(LogLevel::Info, "Peak used memory:        {0}", m_Stats.PeakUsed);
		logger.Log(LogLevel::Info, "Untracked memory used:   {0}", m_Stats.UntrackedMemoryAllocated);
		logger.Log(LogLevel::Info, "Total allocations count: {0}", m_Stats.TotalAllocations);
		logger.Log(LogLevel::Info, "Total frees count:       {0}", m_Stats.TotalFrees);

		for (int i = 0; i < static_cast<int>(MemoryTag::Count); ++i)
		{
			MemoryTag tag = static_cast<MemoryTag>(i);
			MemoryTagStats tagStats= m_Stats.TagStats[i];

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

		usize untrackedMemory = m_Stats.UntrackedMemoryAllocated;

		std::vector<AllocInfo> leaksVector;

		logger.Log(LogLevel::Info, "================== Memory Leaks Report ==================");

		for (const auto& allocInfo : m_ActiveAllocations | std::views::values)
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

			std::ranges::sort(leaksVector, std::less());

			if (!leaksVector.empty())
			{
				for (size_t i = 0; i < leaksVector.size(); ++i)
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

	const MemoryStats& MemoryTracker::GetStats() const
	{
		return m_Stats;
	}

	bool MemoryTracker::IsEnabled()
	{
		return GMemoryTrackerEnabled;
	}

	void MemoryTracker::ClearStats()
	{
		if (!IsEnabled())
			return;

		m_Stats.TotalAllocated = 0;
		m_Stats.CurrentUsed = 0;
		m_Stats.PeakUsed = 0;
		m_Stats.TotalAllocations = 0;
		m_Stats.TotalFrees = 0;

		for (auto& tagStat : m_Stats.TagStats)
		{
			tagStat = {};
		}
	}

	void MemoryTracker::TrackReallocation(void* oldPtr, const void* newPtr, uint64 newSize, const char* filename, int line)
	{
		if (!GMemoryTrackerEnabled)
			return;

		uint64 oldSize = 0;
		MemoryTag tag = MemoryTag::Default;

		if (oldPtr)
		{
			auto it = m_ActiveAllocations.find(oldPtr);
			if (it != m_ActiveAllocations.end()) {
				oldSize = it->second.Size;
				tag = it->second.Tag;
				m_ActiveAllocations.erase(it);
			}
			else
			{
				Log::Warn("TrackReallocation: old pointer {} not found in active allocations", oldPtr);
				auto tagStack = GetMemoryTagsStack();
				tag = tagStack.empty() ? MemoryTag::Default : tagStack.top();
			}
		}
		else
		{
			auto tagStack = GetMemoryTagsStack();
			tag = tagStack.empty() ? MemoryTag::Default : tagStack.top();
		}

		AllocInfo info;
		info.Size = newSize;
		info.Address = newPtr;
		info.Tag = tag;
		info.Line = line;
		info.Filename = filename;
		m_ActiveAllocations[newPtr] = info;

		const int64 delta = static_cast<int64>(newSize) - static_cast<int64>(oldSize);

		if (delta > 0)
			m_Stats.TotalAllocated += static_cast<uint64>(delta);
		
		m_Stats.CurrentUsed += delta;
		m_Stats.PeakUsed = Math::Max(m_Stats.PeakUsed, m_Stats.CurrentUsed);

		MemoryTagStats& t = m_Stats.TagStats[static_cast<int>(tag)];

		if (delta > 0)
			t.TotalAllocated += static_cast<uint64>(delta);
		
		t.CurrentUsed += delta;
		t.PeakUsed = Math::Max(t.PeakUsed, t.CurrentUsed);
	}
}
