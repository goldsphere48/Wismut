#pragma once

#include "Core/CoreTypes.h"

namespace Wi
{
	class TimeSpan
	{
	public:
		TimeSpan(uint64 milliseconds)
			: m_Milliseconds(milliseconds)
		{

		}

		uint64 GetMilliseconds() const
		{
			return m_Milliseconds;
		}

		uint64 GetSeconds() const
		{
			return m_Milliseconds / 1000;
		}

		uint64 GetMinutes() const
		{
			return m_Milliseconds / 1000 / 60;
		}

		uint64 GetHours() const
		{
			return m_Milliseconds / 1000 / 60 / 60;
		}

	private:
		uint64 m_Milliseconds;
	};

	class DateTime
	{
	public:
		DateTime(TimeSpan timePoint)
			: m_TimePoint(timePoint)
		{
			
		}

		uint32 GetHours() const
		{
			return m_TimePoint.GetHours() % 24;
		}

		uint32 GetMinutes() const
		{
			return m_TimePoint.GetMinutes() % 60;
		}

		uint32 GetSeconds() const
		{
			return m_TimePoint.GetSeconds() % 60;
		}

		static DateTime FromMilliseconds(uint64 milliseconds)
		{
			return { TimeSpan(milliseconds) };
		}

	private:
		TimeSpan m_TimePoint;
	};
}
