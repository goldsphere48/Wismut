#pragma once

#include "Core/CoreTypes.h"

namespace Wi
{
	class TimeSpan
	{
	public:
		TimeSpan(u64 milliseconds)
			: m_Milliseconds(milliseconds)
		{

		}

		u64 GetMilliseconds() const
		{
			return m_Milliseconds;
		}

		u64 GetSeconds() const
		{
			return m_Milliseconds / 1000;
		}

		u64 GetMinutes() const
		{
			return m_Milliseconds / 1000 / 60;
		}

		u64 GetHours() const
		{
			return m_Milliseconds / 1000 / 60 / 60;
		}

	private:
		u64 m_Milliseconds;
	};

	class DateTime
	{
	public:
		DateTime(TimeSpan timePoint)
			: m_TimePoint(timePoint)
		{
			
		}

		u32 GetHours() const
		{
			return m_TimePoint.GetHours() % 24;
		}

		u32 GetMinutes() const
		{
			return m_TimePoint.GetMinutes() % 60;
		}

		u32 GetSeconds() const
		{
			return m_TimePoint.GetSeconds() % 60;
		}

		static DateTime FromMilliseconds(u64 milliseconds)
		{
			return { TimeSpan(milliseconds) };
		}

	private:
		TimeSpan m_TimePoint;
	};
}
