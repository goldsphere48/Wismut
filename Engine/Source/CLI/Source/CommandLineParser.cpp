#include "CommandLineParser.h"
#include <sstream>

CommandParameters CommandLineParser::Parse(int argc, char* argv[])
{
	CommandParameters result;

	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (arg.starts_with("--"))
		{
			size_t eqPos = arg.find('=');
			if (eqPos != std::string::npos)
			{
				std::string key = arg.substr(2, eqPos - 2);
				std::string value = arg.substr(eqPos + 1);
				result.Arguments[key] = value;
			}
			else
			{
				std::string key = arg.substr(2);
				result.Arguments[key] = "";
			}
		}
		else
		{
			if (result.Сommand.empty())
				result.Сommand = arg;
		}
	}

	return result;
}
