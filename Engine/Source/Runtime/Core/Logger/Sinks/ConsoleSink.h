#pragma once

#include "Core/Logger/ILoggerSink.h"

namespace Wi
{
	class ConsoleSink final : public ILoggerSink
	{
	public:
		void Log(const LogEntry& entry) override;
	};
}
