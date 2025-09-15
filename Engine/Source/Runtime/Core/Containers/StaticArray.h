#pragma once

#include <type_traits>

#include "Core/Assertion.h"
#include "Core/CoreTypes.h"
#include "RangedForPtrIterator.h"

namespace Wi
{
	namespace Private
	{
		template<typename T, uint32 Size, typename... Args>
		concept StaticArrayCtorArgs =
			(sizeof...(Args) > 0 && sizeof...(Args) <= Size) &&
			(std::constructible_from<T, Args&&> && ...) &&
			((sizeof...(Args) == Size) || std::default_initializable<T>);
	}

	template<typename TElementType, uint32 Size, uint32 Alignment = alignof(TElementType)>
	class alignas(Alignment) TStaticArray
	{
		static_assert((Alignment % alignof(TElementType) == 0), "Alignment must be a multiple of alignof(TElementType)");
		static_assert((Alignment & (Alignment - 1)) == 0, "Alignment must be a power of two");
		static_assert(Size > 0, "Size must be > 0");
	public:
		[[nodiscard]] TStaticArray() = default;

		template<typename ...Args> requires Private::StaticArrayCtorArgs<TElementType, Size, Args...>
		[[nodiscard]] constexpr TStaticArray(Args&& ...args)
			: m_Elements { TElementType(std::forward<Args>(args))... }
		{
			
		}

		static constexpr TStaticArray Filled(const TElementType& v)
		{
			TStaticArray a{};
			a.Fill(v);
			return a;
		}

		constexpr void Fill(const TElementType& value)
		{
			for (uint32 i = 0; i < Size; ++i)
			{
				m_Elements[i] = value;
			}
		}

		[[nodiscard]] constexpr TStaticArray(TStaticArray&& other) = default;
		[[nodiscard]] constexpr TStaticArray(const TStaticArray& other) = default;
		constexpr TStaticArray& operator=(TStaticArray&& other) = default;
		constexpr TStaticArray& operator=(const TStaticArray& other) = default;

		constexpr TElementType& operator[](uint32 index) noexcept
		{
			CORE_CHECK(index < Size)
			return m_Elements[index];
		}

		constexpr const TElementType& operator[](uint32 index) const noexcept
		{
			CORE_CHECK(index < Size)
			return m_Elements[index];
		}

		static constexpr uint32 Count() noexcept
		{
			return Size;
		}

		[[nodiscard]] static constexpr bool Empty()
		{
			return Size == 0;
		}

		[[nodiscard]] constexpr TElementType* GetData()
		{
			static_assert((alignof(TElementType) % Alignment) == 0, "GetData() cannot be called on a TStaticArray with non-standard alignment");
			return m_Elements;
		}

		using Iterator				= TRangedForPtrIterator<TElementType>;
		using ConstIterator			= TRangedForPtrIterator<const TElementType>;
		using ReverseIterator		= TRangedForPtrIterator<TElementType, true>;
		using ConstReverseIterator	= TRangedForPtrIterator<const TElementType, true>;

		static_assert(std::contiguous_iterator<Iterator>);
		static_assert(std::contiguous_iterator<ConstIterator>);
		static_assert(std::random_access_iterator<ReverseIterator>);
		static_assert(std::random_access_iterator<ConstReverseIterator>);

		Iterator begin() noexcept
		{
			return Iterator(m_Elements);
		}

		Iterator end() noexcept
		{
			return Iterator(m_Elements + Size);
		}

		ConstIterator begin() const noexcept
		{
			return ConstIterator(m_Elements);
		}

		ConstIterator end() const noexcept
		{
			return ConstIterator(m_Elements + Size);
		}

		ReverseIterator rbegin() noexcept
		{
			return ReverseIterator(m_Elements + Size);
		}

		ReverseIterator rend() noexcept
		{
			return ReverseIterator(m_Elements);
		}

		ConstReverseIterator rbegin() const noexcept
		{
			return ConstReverseIterator(m_Elements + Size);
		}

		ConstReverseIterator rend() const noexcept
		{
			return ConstReverseIterator(m_Elements);
		}

	private:
		TElementType m_Elements[Size];
	};
}
