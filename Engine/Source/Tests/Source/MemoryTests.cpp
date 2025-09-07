#include <gtest/gtest.h>

#include "Core/Memory/Memory.h"
#include "Core/Memory/MemoryTracker.h"
#include "Core/Templates/AlignTemplates.h"

class MemorySystemTest : public ::testing::Test {
protected:
	void SetUp() override
	{
		Wi::Memory::Initialize();
		Wi::MemoryTracker::ClearStats();
	}

	void TearDown() override
	{
	}

	uint64 GetAlignedSize(uint64 size) const {
		constexpr uint64 DEFAULT_ALIGNMENT = 16;
		return Wi::Align(size, DEFAULT_ALIGNMENT);
	}
};

// Verifies the basic allocation and deallocation of a single object.
TEST_F(MemorySystemTest, BasicAllocationAndDeallocation) {
	const auto& initialStats = Wi::MemoryTracker::GetStats();
	ASSERT_EQ(initialStats.CurrentUsed, 0);
	ASSERT_EQ(initialStats.TotalAllocations, 0);

	int* ptr = new int(42);
	ASSERT_NE(ptr, nullptr);
	EXPECT_EQ(*ptr, 42);

	const auto& statsAfterAlloc = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(statsAfterAlloc.CurrentUsed, GetAlignedSize(sizeof(int)));
	EXPECT_EQ(statsAfterAlloc.TotalAllocations, 1);

	delete ptr;

	const auto& finalStats = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(finalStats.CurrentUsed, 0);
	EXPECT_EQ(finalStats.TotalAllocations, 1);
	EXPECT_EQ(finalStats.TotalFrees, 1);
}

// Verifies the allocation and deallocation of an array.
TEST_F(MemorySystemTest, ArrayAllocationAndDeallocation) {
	const size_t arraySize = 100;

	int* arr = new int[arraySize];
	ASSERT_NE(arr, nullptr);

	for (size_t i = 0; i < arraySize; ++i) {
		arr[i] = static_cast<int>(i);
	}
	for (size_t i = 0; i < arraySize; ++i) {
		EXPECT_EQ(arr[i], static_cast<int>(i));
	}

	const auto& statsAfterAlloc = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(statsAfterAlloc.CurrentUsed, GetAlignedSize(sizeof(int) * arraySize));
	EXPECT_EQ(statsAfterAlloc.TotalAllocations, 1);

	delete[] arr;

	const auto& finalStats = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(finalStats.CurrentUsed, 0);
	EXPECT_EQ(finalStats.TotalFrees, 1);
}

// Checks if memory tracking statistics (CurrentUsed, PeakUsed, etc.) are updated correctly.
TEST_F(MemorySystemTest, MemoryTrackingStatsAreCorrect) {
	void* p1 = WI_ALLOC(10);
	const auto& stats1 = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(stats1.TotalAllocations, 1);
	EXPECT_EQ(stats1.TotalFrees, 0);
	EXPECT_EQ(stats1.CurrentUsed, 16);
	EXPECT_EQ(stats1.PeakUsed, 16);

	void* p2 = WI_ALLOC(40);
	const auto& stats2 = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(stats2.TotalAllocations, 2);
	EXPECT_EQ(stats2.CurrentUsed, 16 + 48);
	EXPECT_EQ(stats2.PeakUsed, 16 + 48);

	WI_FREE(p1);
	const auto& stats3 = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(stats3.TotalFrees, 1);
	EXPECT_EQ(stats3.CurrentUsed, 48);
	EXPECT_EQ(stats3.PeakUsed, 16 + 48);

	WI_FREE(p2);
	const auto& stats4 = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(stats4.TotalFrees, 2);
	EXPECT_EQ(stats4.CurrentUsed, 0);
	EXPECT_EQ(stats4.PeakUsed, 16 + 48);
}

// Ensures that the allocator respects memory alignment requests.
TEST_F(MemorySystemTest, AlignmentCheck) {
	const uint64 alignment = 64;
	void* ptr = WI_ALLOC_ALIGNED(100, alignment);
	ASSERT_NE(ptr, nullptr);

	uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
	EXPECT_EQ(address % alignment, 0);

	WI_FREE(ptr);
}

// Intentionally leaks memory to confirm that the leak detector reports it upon shutdown.
TEST_F(MemorySystemTest, LeakDetectionIsTriggeredOnPurpose) {
	int* leaked_ptr = new int(12345);
	SUCCEED() << "This test intentionally leaks memory to verify leak detection logging.";
}

TEST_F(MemorySystemTest, NoLeaksReportedOnCleanShutdown) {
	int* ptr = new int(10);
	delete ptr;
	SUCCEED() << "This test should produce no leak reports.";
}

// Checks that allocations are correctly categorized using MemoryTagScope.
TEST_F(MemorySystemTest, MemoryTaggingWorks) {
	const auto TEST_TAG = Wi::MemoryTag::Rendering;

	const auto& initialStats = Wi::MemoryTracker::GetStats();
	const auto initialDefaultTagAllocs = initialStats.TagStats[static_cast<int>(Wi::MemoryTag::Default)].TotalAllocations;
	const auto initialTestTagAllocs = initialStats.TagStats[static_cast<int>(TEST_TAG)].TotalAllocations;

	{
		MEMORY_TAG_SCOPE(TEST_TAG);

		int* ptr = new int(1);

		const auto& statsInScope = Wi::MemoryTracker::GetStats();

		EXPECT_EQ(statsInScope.TagStats[static_cast<int>(Wi::MemoryTag::Default)].TotalAllocations, initialDefaultTagAllocs);

		EXPECT_EQ(statsInScope.TagStats[static_cast<int>(TEST_TAG)].TotalAllocations, initialTestTagAllocs + 1);
		EXPECT_GT(statsInScope.TagStats[static_cast<int>(TEST_TAG)].CurrentUsed, 0);

		delete ptr;
	}

	const auto& finalStats = Wi::MemoryTracker::GetStats();
	EXPECT_EQ(finalStats.TagStats[static_cast<int>(TEST_TAG)].CurrentUsed, 0);
}
