#include <gtest/gtest.h>
#include "Core/Memory/Memory.h"
#include "Core/Memory/MemoryTracker.h"
#include <thread>
#include <vector>
#include <cstring>

using namespace Wi;

class MemoryTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		Memory::GetInstance()->Initialize();
	}

	void TearDown() override
	{
		Memory::GetInstance()->Shutdown();
	}
};

// Базовые тесты аллокации
TEST_F(MemoryTest, BasicAllocation)
{
	void* ptr = WI_ALLOC(100);
	ASSERT_NE(ptr, nullptr);
	WI_FREE(ptr);
}

TEST_F(MemoryTest, ZeroSizeAllocation)
{
	void* ptr = WI_ALLOC(0);
	// Поведение может варьироваться - либо nullptr, либо валидный указатель
	if (ptr != nullptr)
	{
		WI_FREE(ptr);
	}
}

TEST_F(MemoryTest, LargeAllocation)
{
	const usize largeSize = 10 * 1024 * 1024; // 10 MB
	void* ptr = WI_ALLOC(largeSize);
	ASSERT_NE(ptr, nullptr);

	// Проверяем, что память доступна для записи
	std::memset(ptr, 0xAB, largeSize);

	WI_FREE(ptr);
}

// Тесты выравнивания
TEST_F(MemoryTest, AlignedAllocation)
{
	const usize alignment = 64;
	void* ptr = WI_ALLOC_ALIGNED(100, alignment);
	ASSERT_NE(ptr, nullptr);
	EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % alignment, 0);
	WI_FREE(ptr);
}

TEST_F(MemoryTest, VariousAlignments)
{
	const usize sizes[] = { 16, 32, 64, 128, 256 };

	for (usize alignment : sizes)
	{
		void* ptr = WI_ALLOC_ALIGNED(100, alignment);
		ASSERT_NE(ptr, nullptr);
		EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % alignment, 0)
			<< "Failed for alignment " << alignment;
		WI_FREE(ptr);
	}
}

TEST_F(MemoryTest, NonPowerOfTwoAlignment)
{
	// Должен вызвать assertion или использовать минимальное выравнивание
	// В зависимости от реализации
	void* ptr = WI_ALLOC_ALIGNED(100, 0);
	ASSERT_NE(ptr, nullptr);
	WI_FREE(ptr);
}

// Тесты реаллокации
TEST_F(MemoryTest, BasicReallocation)
{
	const usize oldSize = 100;
	const usize newSize = 200;

	void* ptr = WI_ALLOC(oldSize);
	ASSERT_NE(ptr, nullptr);

	// Заполняем данными
	std::memset(ptr, 0xAB, oldSize);

	void* newPtr = WI_REALLOC(ptr, oldSize, newSize);
	ASSERT_NE(newPtr, nullptr);

	// Проверяем, что старые данные сохранились
	unsigned char* bytePtr = static_cast<unsigned char*>(newPtr);
	for (usize i = 0; i < oldSize; ++i)
	{
		EXPECT_EQ(bytePtr[i], 0xAB);
	}

	WI_FREE(newPtr);
}

TEST_F(MemoryTest, ReallocationShrink)
{
	const usize oldSize = 200;
	const usize newSize = 100;

	void* ptr = WI_ALLOC(oldSize);
	ASSERT_NE(ptr, nullptr);

	std::memset(ptr, 0xCD, oldSize);

	void* newPtr = WI_REALLOC(ptr, oldSize, newSize);
	ASSERT_NE(newPtr, nullptr);

	// Проверяем, что данные сохранились
	unsigned char* bytePtr = static_cast<unsigned char*>(newPtr);
	for (usize i = 0; i < newSize; ++i)
	{
		EXPECT_EQ(bytePtr[i], 0xCD);
	}

	WI_FREE(newPtr);
}

TEST_F(MemoryTest, ReallocationAligned)
{
	const usize alignment = 64;
	const usize oldSize = 100;
	const usize newSize = 200;

	void* ptr = WI_ALLOC_ALIGNED(oldSize, alignment);
	ASSERT_NE(ptr, nullptr);
	EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % alignment, 0);

	void* newPtr = WI_REALLOC_ALIGNED(ptr, oldSize, newSize, alignment);
	ASSERT_NE(newPtr, nullptr);
	EXPECT_EQ(reinterpret_cast<uintptr_t>(newPtr) % alignment, 0);

	WI_FREE(newPtr);
}

TEST_F(MemoryTest, ReallocationNullptr)
{
	// Реаллокация nullptr должна работать как обычная аллокация
	void* ptr = WI_REALLOC(nullptr, 0, 100);
	ASSERT_NE(ptr, nullptr);
	WI_FREE(ptr);
}

// Тесты операторов new/delete
TEST_F(MemoryTest, NewOperator)
{
	int* ptr = WI_NEW int(42);
	ASSERT_NE(ptr, nullptr);
	EXPECT_EQ(*ptr, 42);
	WI_DELETE(ptr);
}

TEST_F(MemoryTest, NewArrayOperator)
{
	const int size = 10;
	int* arr = WI_NEW int[size];
	ASSERT_NE(arr, nullptr);

	for (int i = 0; i < size; ++i)
	{
		arr[i] = i;
	}

	for (int i = 0; i < size; ++i)
	{
		EXPECT_EQ(arr[i], i);
	}

	WI_DELETE_ARR(arr);
}

TEST_F(MemoryTest, NewComplexObject)
{
	struct TestObject
	{
		int a;
		double b;
		char c[10];

		TestObject() : a(100), b(3.14), c{ 0 } {}
	};

	TestObject* obj = WI_NEW TestObject();
	ASSERT_NE(obj, nullptr);
	EXPECT_EQ(obj->a, 100);
	EXPECT_DOUBLE_EQ(obj->b, 3.14);
	WI_DELETE(obj);
}

// Тесты множественных аллокаций
TEST_F(MemoryTest, MultipleAllocations)
{
	const int count = 100;
	std::vector<void*> pointers;

	for (int i = 0; i < count; ++i)
	{
		void* ptr = WI_ALLOC(100 + i);
		ASSERT_NE(ptr, nullptr);
		pointers.push_back(ptr);
	}

	// Проверяем, что все указатели уникальны
	for (size_t i = 0; i < pointers.size(); ++i)
	{
		for (size_t j = i + 1; j < pointers.size(); ++j)
		{
			EXPECT_NE(pointers[i], pointers[j]);
		}
	}

	for (void* ptr : pointers)
	{
		WI_FREE(ptr);
	}
}

// Тест утечек памяти (с использованием MemoryTracker)
TEST_F(MemoryTest, MemoryLeakDetection)
{
	// Выделяем и освобождаем память
	void* ptr1 = WI_ALLOC(100);
	void* ptr2 = WI_ALLOC(200);

	WI_FREE(ptr1);
	WI_FREE(ptr2);

	// После освобождения не должно быть утечек
	// Здесь можно добавить проверку через MemoryTracker
	// если в нем есть соответствующие методы
}

TEST_F(MemoryTest, DoubleFree)
{
	void* ptr = WI_ALLOC(100);
	ASSERT_NE(ptr, nullptr);
	WI_FREE(ptr);

	// Второй вызов Free может привести к краху или undefined behavior
	// В продакшене должна быть защита от этого
	// EXPECT_DEATH(WI_FREE(ptr), ".*"); // Раскомментировать если есть защита
}

// Стресс-тесты
TEST_F(MemoryTest, StressTestRandomAllocations)
{
	const int iterations = 1000;
	std::vector<void*> pointers;

	for (int i = 0; i < iterations; ++i)
	{
		usize size = (rand() % 1000) + 1;
		void* ptr = WI_ALLOC(size);
		ASSERT_NE(ptr, nullptr);
		pointers.push_back(ptr);

		// Периодически освобождаем память
		if (i % 10 == 0 && !pointers.empty())
		{
			WI_FREE(pointers.back());
			pointers.pop_back();
		}
	}

	// Освобождаем оставшуюся память
	for (void* ptr : pointers)
	{
		WI_FREE(ptr);
	}
}

// Тесты инициализации/деинициализации
TEST(MemoryInitTest, DoubleInitialization)
{
	Memory::GetInstance()->Initialize();
	// Повторная инициализация должна вызвать assertion
	// EXPECT_DEATH(Memory::GetInstance()->Initialize(), ".*");
	Memory::GetInstance()->Shutdown();
}

TEST(MemoryInitTest, ShutdownWithoutInitialization)
{
	// Shutdown без Initialize должен вызвать assertion
	// EXPECT_DEATH(Memory::GetInstance()->Shutdown(), ".*");
}

// Тесты граничных случаев
TEST_F(MemoryTest, AllocationBoundaryConditions)
{
	// Минимальный размер
	void* ptr1 = WI_ALLOC(1);
	ASSERT_NE(ptr1, nullptr);
	WI_FREE(ptr1);

	// Степени двойки
	for (int i = 0; i < 20; ++i)
	{
		usize size = 1ULL << i;
		void* ptr = WI_ALLOC(size);
		ASSERT_NE(ptr, nullptr) << "Failed for size " << size;
		WI_FREE(ptr);
	}
}

TEST_F(MemoryTest, DataIntegrity)
{
	const usize size = 1000;
	void* ptr = WI_ALLOC(size);
	ASSERT_NE(ptr, nullptr);

	unsigned char* bytePtr = static_cast<unsigned char*>(ptr);

	// Записываем паттерн
	for (usize i = 0; i < size; ++i)
	{
		bytePtr[i] = static_cast<unsigned char>(i % 256);
	}

	// Проверяем паттерн
	for (usize i = 0; i < size; ++i)
	{
		EXPECT_EQ(bytePtr[i], static_cast<unsigned char>(i % 256));
	}

	WI_FREE(ptr);
}

// TODO: Раскомментировать после добавления потокобезопасности
/*
TEST_F(MemoryTest, ThreadSafety)
{
	const int threadsCount = 10;
	const int allocationsPerThread = 100;

	auto allocateFunction = [allocationsPerThread]()
	{
		std::vector<void*> pointers;
		for (int i = 0; i < allocationsPerThread; ++i)
		{
			void* ptr = WI_ALLOC(100);
			ASSERT_NE(ptr, nullptr);
			pointers.push_back(ptr);
		}

		for (void* ptr : pointers)
		{
			WI_FREE(ptr);
		}
	};

	std::vector<std::thread> threads;
	for (int i = 0; i < threadsCount; ++i)
	{
		threads.emplace_back(allocateFunction);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}
*/
