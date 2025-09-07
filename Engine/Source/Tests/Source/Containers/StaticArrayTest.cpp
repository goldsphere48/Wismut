#include <gtest/gtest.h>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <type_traits>
#include <span>
#include <cstdint>

#include "Core/Containers/StaticArray.h"

using Wi::TStaticArray;

// ===============================
// Compile-time (concept/traits) checks
// ===============================

// The container should model the basic range concepts.
static_assert(std::ranges::range<TStaticArray<int, 4>>, "TStaticArray must satisfy range");
static_assert(std::ranges::sized_range<TStaticArray<int, 4>>, "TStaticArray must have constant size");
static_assert(std::ranges::common_range<TStaticArray<int, 4>>, "begin/end types must be the same");
static_assert(std::ranges::random_access_range<TStaticArray<int, 4>>, "iterator must support random access");
static_assert(std::ranges::contiguous_range<TStaticArray<int, 4>>, "iterator must be contiguous");

// The same must hold for const-qualified containers.
static_assert(std::ranges::range<const TStaticArray<int, 4>>);
static_assert(std::ranges::sized_range<const TStaticArray<int, 4>>);
static_assert(std::ranges::common_range<const TStaticArray<int, 4>>);
static_assert(std::ranges::random_access_range<const TStaticArray<int, 4>>);
static_assert(std::ranges::contiguous_range<const TStaticArray<int, 4>>);

// It is not a view and not a borrowed_range by value; borrowing only applies for lvalues.
static_assert(!std::ranges::view<TStaticArray<int, 4>>, "TStaticArray is an owning container, not a view");
static_assert(!std::ranges::borrowed_range<TStaticArray<int, 4>>, "By-value use should not be borrowed");
static_assert(std::ranges::borrowed_range<TStaticArray<int, 4>&>, "Lvalue references should be borrowed");

// For trivially copyable element types, the wrapper should keep trivial special members.
static_assert(std::is_trivially_copy_constructible_v<TStaticArray<int, 4>>);
static_assert(std::is_trivially_move_constructible_v<TStaticArray<int, 4>>);
static_assert(std::is_trivially_copy_assignable_v<TStaticArray<int, 4>>);
static_assert(std::is_trivially_move_assignable_v<TStaticArray<int, 4>>);

// Alignment parameter must be reflected in the type's alignment.
static_assert(alignof(TStaticArray<int, 4, 64>) == 64, "Custom alignment must be honored");

// ===============================
// NEW: Compile-time checks for Empty() and GetData()
// ===============================

// Empty() is constexpr and, given Size > 0 invariant, must be false.
static_assert(!TStaticArray<int, 1>::Empty(), "Empty() must be false when Size > 0");
static_assert(!TStaticArray<int, 4>::Empty(), "Empty() must be false when Size > 0");

// GetData() should NOT be callable on a const object because it's a non-const member.
template <class A>
concept HasConstGetData = requires (const A & ca) { ca.GetData(); };

static_assert(!HasConstGetData<TStaticArray<int, 4>>,
	"GetData() is non-const and must not be callable on const arrays");

// Note on "non-standard" alignment: GetData() contains a static_assert that intentionally
// rejects calling it when Alignment != alignof(TElementType). We cannot write a negative
// test that compiles, but we document the intent below:
// 
//   using A32 = TStaticArray<int, 8, 32>;
//   A32 a;
//   (void)a.GetData(); // <-- this should FAIL to compile by design
//

// ===============================
// Runtime tests
// ===============================

TEST(TStaticArray, CountAndIndexing)
{
	// Purpose: verify Count() reports the static size and operator[] provides mutable access.
	TStaticArray<int, 5> a;
	EXPECT_EQ(decltype(a)::Count(), 5u);

	// Fill all elements with a sentinel value and read them back.
	a.Fill(7);
	for (std::uint32_t i = 0; i < decltype(a)::Count(); ++i)
		EXPECT_EQ(a[i], 7);
}

TEST(TStaticArray, FilledFactory)
{
	// Purpose: ensure the Filled() factory constructs an array with all values set to the given argument.
	auto a = TStaticArray<int, 3>::Filled(42);
	for (auto v : a)
		EXPECT_EQ(v, 42);
}

TEST(TStaticArray, Constructor_ExactArgs)
{
	// Purpose: the variadic constructor should accept exactly Size args and initialize in order.
	TStaticArray<int, 3> a(1, 2, 3);
	EXPECT_EQ(a[0], 1);
	EXPECT_EQ(a[1], 2);
	EXPECT_EQ(a[2], 3);
}

struct Foo
{
	// Purpose: type with default-init state distinct from value-init to detect default initialization.
	int x;
	Foo() : x(-1) {}
	explicit Foo(int v) : x(v) {}
};

TEST(TStaticArray, Constructor_PartialArgs_DefaultInitializeRest)
{
	// Purpose: when fewer than Size args are passed, remaining elements must be value-initialized.
	// For int, value-init yields 0.
	TStaticArray<int, 4> ai(5);
	EXPECT_EQ(ai[0], 5);
	EXPECT_EQ(ai[1], 0);
	EXPECT_EQ(ai[2], 0);
	EXPECT_EQ(ai[3], 0);

	// For Foo, value-init calls Foo(), yielding x == -1.
	TStaticArray<Foo, 3> af(Foo{ 10 });
	EXPECT_EQ(af[0].x, 10);
	EXPECT_EQ(af[1].x, -1);
	EXPECT_EQ(af[2].x, -1);
}

TEST(TStaticArray, Iteration_AndRangesDistance)
{
	// Purpose: forward iteration via range-for, ranges::distance/size, and contiguous data() access.
	TStaticArray<int, 5> a(1, 2, 3, 4, 5);
	int sum = 0;
	for (int v : a) sum += v;
	EXPECT_EQ(sum, 15);

	// Confirm sized_range / contiguous_range interoperability.
	EXPECT_EQ(std::ranges::distance(a), 5);
	EXPECT_EQ(std::ranges::size(a), 5u);
	ASSERT_NE(std::ranges::data(a), nullptr);
	EXPECT_EQ(std::ranges::data(a), &a[0]);
}

TEST(TStaticArray, ConstIteration)
{
	// Purpose: ensure const-qualified arrays are iterable and yield const elements.
	const TStaticArray<int, 4> a(1, 2, 3, 4);
	int prod = 1;
	for (int v : a) prod *= v;
	EXPECT_EQ(prod, 24);
}

TEST(TStaticArray, ReverseIteration_Methods)
{
	// Purpose: validate rbegin()/rend() produce reverse traversal in correct order.
	TStaticArray<int, 4> a(1, 2, 3, 4);

	std::vector<int> rev;
	for (int& it : std::ranges::reverse_view(a))
		rev.push_back(it);

	ASSERT_EQ(rev.size(), 4u);
	EXPECT_EQ((rev == std::vector{ 4, 3, 2, 1 }), true);
}

TEST(TStaticArray, Ranges_AlgorithmsAndViews)
{
	// Purpose: TStaticArray must plug into std::ranges algorithms and views.
	TStaticArray<int, 6> a(5, 1, 4, 2, 6, 3);

	// ranges::sort should sort in-place.
	std::ranges::sort(a);
	EXPECT_TRUE(std::ranges::is_sorted(a));
	EXPECT_TRUE(std::ranges::equal(a, std::array{ 1, 2, 3, 4, 5, 6 }));

	// views::transform produces a lazy view; copy it out and verify content.
	auto squared = a | std::views::transform([](int x) { return x * x; });
	std::vector<int> sqv;
	std::ranges::copy(squared, std::back_inserter(sqv));
	EXPECT_TRUE((sqv == std::vector{ 1, 4, 9, 16, 25, 36 }));

	// views::reverse must work thanks to random access + bidirectional iteration.
	auto reversed = a | std::views::reverse;
	std::vector<int> rev;
	std::ranges::copy(reversed, std::back_inserter(rev));
	EXPECT_TRUE((rev == std::vector{ 6, 5, 4, 3, 2, 1 }));
}

TEST(TStaticArray, Ranges_CopyToSpanAndVector)
{
	// Purpose: demonstrate interop via contiguous_range: construct std::span and copy to std::vector.
	TStaticArray<int, 5> a(10, 20, 30, 40, 50);

	// Build span from data/size; confirm indexing works.
	std::span<int> sp(std::ranges::data(a), std::ranges::size(a));
	ASSERT_EQ(sp.size(), 5u);
	EXPECT_EQ(sp[2], 30);

	// Copy through ranges::copy into a std::vector.
	std::vector<int> v;
	v.resize(a.Count());
	std::ranges::copy(a, v.begin());
	EXPECT_TRUE((v == std::vector{ 10, 20, 30, 40, 50 }));
}

TEST(TStaticArray, AlignmentParameter)
{
	// Purpose: verify that custom alignment on the container type is respected at runtime as well.
	TStaticArray<int, 8, 32> a32;
	EXPECT_EQ(alignof(decltype(a32)), 32u);

	// Ensure element access still works after Fill under custom alignment.
	a32.Fill(9);
	for (auto x : a32) EXPECT_EQ(x, 9);
}

// ===============================
// NEW: Runtime tests for Empty() and GetData()
// ===============================

TEST(TStaticArray, EmptyAndStdRangesEmpty)
{
	// Purpose: Empty() mirrors the static size; with Size > 0 it must be false.
	TStaticArray<int, 3> a;
	EXPECT_FALSE(decltype(a)::Empty());
	EXPECT_FALSE(std::ranges::empty(a));
}

TEST(TStaticArray, GetData_DefaultAlignment)
{
	// Purpose: when Alignment == alignof(TElementType), GetData() is available and returns a raw pointer
	// to the first element; it must match &a[0] and std::ranges::data(a). The pointer must be correctly aligned.
	TStaticArray<int, 4> a(1, 2, 3, 4);
	int* p = a.GetData();

	ASSERT_NE(p, nullptr);
	EXPECT_EQ(p, &a[0]);
	EXPECT_EQ(p, std::ranges::data(a));

	// Basic alignment sanity check on the returned pointer.
	auto address = reinterpret_cast<std::uintptr_t>(p);
	EXPECT_EQ(address % alignof(int), 0u);
}
