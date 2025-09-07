#pragma once

#include <iterator>
#include <type_traits>
#include <compare>
#include <cstddef>

namespace Wi
{
	template<typename TElementType, bool bReverse = false, typename DesiredConcept = std::contiguous_iterator_tag>
	class TRangedForPtrIterator
	{
		template<class C> struct is_concept_tag : std::false_type {};
		template<> struct is_concept_tag<std::random_access_iterator_tag> : std::true_type {};
		template<> struct is_concept_tag<std::contiguous_iterator_tag> : std::true_type {};
		static_assert(is_concept_tag<DesiredConcept>::value, "DesiredConcept must be a valid iterator tag");

		using FinalConcept =
			std::conditional_t<bReverse&& std::is_same_v<DesiredConcept, std::contiguous_iterator_tag>,
			std::random_access_iterator_tag,
			DesiredConcept>;

	public:
		using difference_type = std::ptrdiff_t;
		using value_type = std::remove_cv_t<TElementType>;

		using iterator_concept = FinalConcept;
		using iterator_category = std::random_access_iterator_tag;

		using pointer = std::add_pointer_t<TElementType>;
		using reference = std::add_lvalue_reference_t<TElementType>;
		using Iterator = TRangedForPtrIterator;

		constexpr explicit TRangedForPtrIterator(TElementType* ptr) noexcept
			: m_Ptr(ptr)
		{

		}

		constexpr TRangedForPtrIterator() noexcept
			: m_Ptr(nullptr)
		{

		}

		constexpr reference operator*() const noexcept
		{
			if constexpr (bReverse)
				return *(m_Ptr - 1);
			else
				return *m_Ptr;
		}

		constexpr pointer operator->() const noexcept
		{
			if constexpr (bReverse)
				return m_Ptr - 1;
			else
				return m_Ptr;
		}

		constexpr Iterator& operator++() noexcept
		{
			if constexpr (bReverse)
				--m_Ptr;
			else
				++m_Ptr;

			return *this;
		}

		constexpr Iterator operator++(int) noexcept
		{
			Iterator result = Iterator(m_Ptr);
			if constexpr (bReverse)
				--m_Ptr;
			else
				++m_Ptr;

			return result;
		}

		constexpr Iterator& operator--() noexcept
		{
			if constexpr (bReverse)
				++m_Ptr;
			else
				--m_Ptr;

			return *this;
		}

		constexpr Iterator operator--(int) noexcept
		{
			Iterator result = Iterator(m_Ptr);

			if constexpr (bReverse)
				++m_Ptr;
			else
				--m_Ptr;

			return result;
		}

		constexpr Iterator& operator+=(difference_type n) noexcept
		{
			if constexpr (bReverse)
				m_Ptr -= n;
			else
				m_Ptr += n;

			return *this;
		}

		constexpr Iterator& operator-=(difference_type n) noexcept
		{
			if constexpr (bReverse)
				m_Ptr += n;
			else
				m_Ptr -= n;

			return *this;
		}

		constexpr reference operator[](difference_type index) const noexcept
		{
			if constexpr (bReverse)
				return *(m_Ptr - index - 1);
			else
				return *(m_Ptr + index);
		}

		constexpr auto operator<=>(const Iterator&) const noexcept = default;

		friend constexpr Iterator operator+(Iterator a, difference_type size) noexcept
		{
			a += size;
			return a;
		}

		friend constexpr Iterator operator+(difference_type size, Iterator a) noexcept
		{
			a += size;
			return a;
		}

		friend constexpr Iterator operator-(Iterator a, difference_type size) noexcept
		{
			a -= size;
			return a;
		}

		friend constexpr difference_type operator-(const Iterator& a, Iterator b) noexcept
		{
			if constexpr (bReverse)
				return b.m_Ptr - a.m_Ptr;
			else
				return a.m_Ptr - b.m_Ptr;
		}

	private:
		pointer m_Ptr;
	};
}
